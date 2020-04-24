/*
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <cx/common.h>
#include <cx/list.h>
#include <HandmadeMath.h>

#include <GL/glex.h>

#define GLEX_HEAP_SIZE_ORDER 26
#define GLEX_HEAP_SIZE (1 << GLEX_HEAP_SIZE_ORDER)

#ifdef GLEX_LOG
typedef enum {
	GLEX_LOG_ERROR = 0x1,
	GLEX_LOG_WARNING = 0x2,
	GLEX_LOG_INFO = 0x4,
	GLEX_LOG_DEBUG = 0x8
} GELXLogType;
#endif

typedef enum {
    GLEX_HEAP_TYPE_STATIC_VERTEX = 0,
    GLEX_HEAP_TYPE_STATIC_VERTEX_INDEX,
    GLEX_HEAP_TYPE_DYNAMIC_VERTEX,
    GLEX_HEAP_TYPE_DYNAMIC_VERTEX_INDEX,
    GLEX_HEAP_TYPE_MAX,
    GLEX_HEAP_TYPE_INVALID
} GLEXHeapType;

typedef struct {
    GLEXHeapType type;
    GLuint id;
    cx_list_node_t node;
    cx_list_t bufferList;
    GLint freeSize;
} GLEXHeap;

typedef struct {
    cx_list_node_t node;
    GLEXHeap *heap;
    GLint offset;
    GLint size;
} GLEXBuffer;

typedef struct {
    GLEXLight *light;
    hmm_mat4 transform;
} GLEXFrameLight;

#define GLEX_FRAME_LIGHT_MAX 1024

typedef struct {
    GLEXMesh *mesh;
    GLEXMaterial *material;
    hmm_mat4 transform;
} GLEXFrameMesh;

#define GLEX_FRAME_MESH_MAX 8192

struct GLEXLight_ {
    int foo;
};

struct GLEXMaterial_ {
    GLuint texture;
};

struct GLEXMesh_ {
    GLEXMeshType type;
	GLenum mode;
    GLuint vao;
    GLEXBuffer vertexBuffer;
    GLint vertexCount;
    GLEXBuffer vertexIndexBuffer;
    GLint vertexIndexCount;
};

typedef void (*GLEXRenderFunc)(void);

struct GLEXContext_ {
#ifdef GLEX_LOG
	GLuint logMask;
#endif

	GLEXRenderMode renderMode;
	GLEXRenderFunc renderFunc;

    cx_list_t heapLists[GLEX_HEAP_TYPE_MAX];
    GLEXHeap *currentHeaps[GLEX_HEAP_TYPE_MAX];

    GLEXMaterial defaultMaterial;
    GLEXMaterial *currentMaterial;

    GLuint program;

    GLint modelMatrixLoc;

    hmm_mat4 viewMatrix;
    GLint viewMatrixLoc;

    hmm_mat4 projectionMatrix;
    GLint projectionMatrixLoc;

    struct {
        GLEXFrameLight lightArray[GLEX_FRAME_LIGHT_MAX];
        int lightCount;
        GLEXFrameMesh meshArray[GLEX_FRAME_MESH_MAX];
        int meshCount;
    } frame;
};

const char *glexVertexShaderSource =
    "#version 330\n"
    "uniform mat4 glex_ViewMatrix;\n"
    "uniform mat4 glex_ProjectionMatrix;\n"
    "uniform mat4 glex_ModelMatrix;\n"
    "layout(location = 0) in vec3 position;\n"
    "layout(location = 1) in vec3 normal;\n"
    "layout(location = 2) in vec2 texCoord;\n"
    "void main(void) {\n"
    "    gl_Position = glex_ProjectionMatrix * glex_ViewMatrix * glex_ModelMatrix * vec4(position, 1.0);\n"
    "}\n";

const char *glexFragmentShaderSource =
    "#version 330\n"
    "void main(void) {\n"
    "    gl_FragColor = vec4(0.8, 0.8, 0.8, 1.0);\n"
    "}\n";

static GLEXContext *glex = NULL;

static void glexFreeBuffer(GLEXBuffer *buffer);

#ifdef GLEX_LOG

static void glexLog(GLuint type, const char *fmt, ...)
{
	va_list args;
	int len;
	char buf[256];
	GLenum glType = GL_DEBUG_TYPE_OTHER;

	if (!GL_KHR_debug)
		return;

	if (glex != NULL && !(glex->logMask & type))
		return;

	switch (type) {
	case GLEX_LOG_ERROR:
		glType = GL_DEBUG_TYPE_ERROR;
		strcpy(buf, "GLEX-E: ");
		break;
	case GLEX_LOG_WARNING:
		strcpy(buf, "GLEX-W: ");
		break;
	case GLEX_LOG_INFO:
		strcpy(buf, "GLEX-I: ");
		break;
	case GLEX_LOG_DEBUG:
		strcpy(buf, "GLEX-D: ");
		break;
	default:
		strcpy(buf, "GLEX-?: ");
		break;
	}

	va_start(args, fmt);
	len = 8 + vsnprintf(buf + 8, sizeof(buf), fmt, args);
	va_end(args);

	glDebugMessageInsert(GL_DEBUG_SOURCE_THIRD_PARTY, glType, 0, GL_DEBUG_SEVERITY_NOTIFICATION, len, buf);
}

#define GLEX_LOGE(fmt, ...) glexLog(GLEX_LOG_ERROR, fmt, ##__VA_ARGS__)
#define GLEX_LOGW(fmt, ...) glexLog(GLEX_LOG_WARNING, fmt, ##__VA_ARGS__)
#define GLEX_LOGI(fmt, ...) glexLog(GLEX_LOG_INFO, fmt, ##__VA_ARGS__)
#define GLEX_LOGD(fmt, ...) glexLog(GLEX_LOG_DEBUG, fmt, ##__VA_ARGS__)

#else

#define GLEX_LOGE(fmt, ...) do { } while (0)
#define GLEX_LOGW(fmt, ...) do { } while (0)
#define GLEX_LOGI(fmt, ...) do { } while (0)
#define GLEX_LOGD(fmt, ...) do { } while (0)

#endif

static GLEXHeapType glexGetHeapType(GLenum target, GLenum usage)
{
    CX_ASSERT(glex != NULL);

    switch (target) {
    case GL_ARRAY_BUFFER:
        if (usage == GL_STATIC_DRAW)
            return GLEX_HEAP_TYPE_STATIC_VERTEX;
        if (usage == GL_DYNAMIC_DRAW)
            return GLEX_HEAP_TYPE_DYNAMIC_VERTEX;
        break;
    case GL_ELEMENT_ARRAY_BUFFER:
        if (usage == GL_STATIC_DRAW)
            return GLEX_HEAP_TYPE_STATIC_VERTEX_INDEX;
        if (usage == GL_DYNAMIC_DRAW)
            return GLEX_HEAP_TYPE_DYNAMIC_VERTEX_INDEX;
        break;
    }

    return GLEX_HEAP_TYPE_INVALID;
}

static GLEXHeap *glexCreateHeap(GLenum target, GLenum usage)
{
    GLEXHeap *heap;
    GLEXHeapType type;

    CX_ASSERT(glex != NULL);

    type = glexGetHeapType(target, usage);
    if (type == GLEX_HEAP_TYPE_INVALID)
        return NULL;

    heap = malloc(sizeof(GLEXHeap));
    if (heap == NULL)
        goto bad0;

    glGenBuffers(1, &heap->id);
    if (heap->id == 0)
        goto bad1;

    glBindBuffer(target, heap->id);
    glBufferData(target, GLEX_HEAP_SIZE, NULL, usage);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    cx_list_node_reset(&heap->node);
    cx_list_add_head(&glex->heapLists[type], &heap->node);
    cx_list_reset(&heap->bufferList);

    heap->type = type;
    heap->freeSize = GLEX_HEAP_SIZE;

    return heap;

bad1:
    free(heap);

bad0:
    return NULL;
}

static void glexFreeHeap(GLEXHeap *heap)
{
    cx_list_node_t *node;
    GLEXBuffer *buffer;

    CX_ASSERT(heap != NULL);
    CX_ASSERT(heap->id > 0);
    
    while((node = cx_list_head(&heap->bufferList)) != cx_list_knot(&heap->bufferList)) {
        buffer = CX_MEMBEROF(node, GLEXBuffer, node);
        glexFreeBuffer(buffer);
    }

    glDeleteBuffers(1, &heap->id);

    cx_list_node_unlink(&heap->node);
    free(heap);
}

static int glexAllocBufferFromHeap(GLEXHeap *heap, GLEXBuffer *buffer, int size)
{
    int pos = 0;
    cx_list_node_t *node;
    GLEXBuffer *used = NULL;
	int found = 0;

    CX_ASSERT(heap != NULL);
    CX_ASSERT(buffer != NULL);
    CX_ASSERT(size > 0);
	CX_ASSERT(size <= GLEX_HEAP_SIZE);

    if (size > heap->freeSize)
        return -1;

    CX_LIST_FOREACH(node, &heap->bufferList) {
        used = CX_MEMBEROF(node, GLEXBuffer, node);
        if ((used->offset - pos) >= size) {
            found = 1;
			break;
		}
        pos = used->offset + used->size;
    }

    cx_list_node_reset(&buffer->node);

    if (!found) {
		if (used != NULL && size > (GLEX_HEAP_SIZE - pos))
			return -1;
		cx_list_add_tail(&heap->bufferList, &buffer->node);
	} else
		cx_list_node_link(&buffer->node, cx_list_node_prev(&used->node), &used->node);

    buffer->offset = pos;
	buffer->size = size;
	buffer->heap = heap;

    heap->freeSize -= size;

    return 0;
}

static int glexAllocBuffer(GLEXBuffer *buffer, int size, GLenum target, GLenum usage)
{
    int ret;
    cx_list_node_t *node;
    GLEXHeap *heap;
    GLEXHeapType type;

    CX_ASSERT(glex != NULL);
    CX_ASSERT(buffer != NULL);
    CX_ASSERT(size > 0);
    
    if (size > GLEX_HEAP_SIZE)
        return -1;

    type = glexGetHeapType(target, usage);
    if (type == GLEX_HEAP_TYPE_INVALID)
        return -1;

    CX_LIST_FOREACH(node, &glex->heapLists[type]) {
        heap = CX_MEMBEROF(node, GLEXHeap, node);
        ret = glexAllocBufferFromHeap(heap, buffer, size);
        if (ret == 0)
            return 0;
    }

    heap = glexCreateHeap(target, usage);
    if (heap == NULL)
        return -1;

    return glexAllocBufferFromHeap(heap, buffer, size);
}

static void glexFreeBuffer(GLEXBuffer *buffer)
{
    CX_ASSERT(buffer != NULL);
    CX_ASSERT(buffer->heap != NULL);
    CX_ASSERT(buffer->size > 0);
    CX_ASSERT(cx_list_node_linked(&buffer->node));

    cx_list_del_node(&buffer->heap->bufferList, &buffer->node);

    buffer->offset = -1;
    buffer->size = 0;
    buffer->heap = NULL;
}

static GLuint glexCreateShader(GLenum type, const char *src)
{
    GLuint id;
    GLint ret;

    CX_ASSERT(src != NULL);

    id = glCreateShader(type);
    if (id == 0)
        return 0;

    glShaderSource(id, 1, &src, NULL);
    glCompileShader(id);
    glGetShaderiv(id, GL_COMPILE_STATUS, &ret);
    if (ret != GL_TRUE) {
        glDeleteShader(id);
        return 0;
    }

    return id;
}

static int glexInitProgram(GLEXContext *context)
{
    GLuint vs;
    GLuint fs;
    GLint ret;

    CX_ASSERT(context != NULL);

	GLEX_LOGD("Creating vertex shader...");
    vs = glexCreateShader(GL_VERTEX_SHADER, glexVertexShaderSource);
    if (vs == 0)
        goto bad0;

    fs = glexCreateShader(GL_FRAGMENT_SHADER, glexFragmentShaderSource);
    if (fs == 0)
        goto bad1;

    context->program = glCreateProgram();
    if (context->program == 0)
        goto bad2;

    glAttachShader(context->program, vs);
    glAttachShader(context->program, fs);

    glLinkProgram(context->program);
    glGetProgramiv(context->program, GL_LINK_STATUS, &ret);
    if (ret != GL_TRUE)
        goto bad3;

    context->modelMatrixLoc = glGetUniformLocation(context->program, "glex_ModelMatrix");
    context->viewMatrixLoc = glGetUniformLocation(context->program, "glex_ViewMatrix");
    context->projectionMatrixLoc = glGetUniformLocation(context->program, "glex_ProjectionMatrix");

    glDeleteShader(vs);
    glDeleteShader(fs);

    return 0;

bad3:
    glDeleteProgram(context->program);
    context->program = 0;

bad2:
    glDeleteShader(fs);

bad1:
    glDeleteShader(vs);

bad0:
    return -1;
}

static void glexShutdownProgram(GLEXContext *context)
{
    CX_ASSERT(context != NULL);
    CX_ASSERT(context->program > 0);

    glDeleteProgram(context->program);
    context->program = 0;

    context->modelMatrixLoc = -1;
    context->viewMatrixLoc = -1;
    context->projectionMatrixLoc = -1;
}

static void glexBindMaterial(GLEXMaterial *material)
{
    CX_ASSERT(glex != NULL);

    if (glex->currentMaterial == material)
        return;

    if (material != NULL) {
        /* TODO */
    } else {
        /* TODO use empty material. */
    }

    glex->currentMaterial = material;
}

static void glexRenderForward(void)
{
    int i;
    GLEXFrameMesh *frameMesh;
	GLEXMesh *mesh;

    glUseProgram(glex->program);

    glUniformMatrix4fv(glex->viewMatrixLoc, 1, GL_FALSE, (const GLfloat *)(glex->viewMatrix.Elements));
    glUniformMatrix4fv(glex->projectionMatrixLoc, 1, GL_FALSE, (const GLfloat *)(glex->projectionMatrix.Elements));

    for (i = 0; i < glex->frame.meshCount; ++i) {
        frameMesh = &glex->frame.meshArray[i];
		mesh = frameMesh->mesh;
        glUniformMatrix4fv(glex->modelMatrixLoc, 1, GL_FALSE,
			(const GLfloat *)(frameMesh->transform.Elements));
        glexBindMaterial(frameMesh->material);
        glBindVertexArray(mesh->vao);
        glDrawElements(GL_TRIANGLES, mesh->vertexIndexCount, GL_UNSIGNED_SHORT,
			(const void *)(GLsizeiptr)(mesh->vertexIndexBuffer.offset));
        glBindVertexArray(0);
    }
}

static void glexRenderNone(void)
{
}

static void glexRenderForwardPlus(void)
{
}

static void glexRenderDeferred(void)
{
}

GLEX_API GLEXContext *glexCreateContext(void)
{
    int i;
    int ret;
    GLEXContext *context;

    context = malloc(sizeof(GLEXContext));
    if (context == NULL)
        goto bad0;

    memset(context, 0, sizeof(GLEXContext));

#ifdef GLEX_LOG
	context->logMask = GLEX_LOG_ERROR | GLEX_LOG_WARNING | GLEX_LOG_INFO;
#ifdef GLEX_DEBUG
	context->logMask |= GLEX_LOG_DEBUG;
#endif
#endif

    ret = glexInitProgram(context);
    if (ret < 0)
        goto bad1;

    for (i = 0; i < GLEX_HEAP_TYPE_MAX; ++i)
        cx_list_reset(&context->heapLists[i]);

	context->renderMode = GLEX_RENDER_MODE_NONE;
	context->renderFunc = glexRenderNone;

    context->viewMatrix = HMM_Mat4d(1.0f);
    context->projectionMatrix = HMM_Mat4d(1.0f);

    context->currentMaterial = NULL;

    return context;

bad1:
    free(context);

bad0:
    return NULL;
}

GLEX_API void glexDeleteContext(GLEXContext *context)
{
    GLEXContext *curr = glex;
    cx_list_node_t *node;
    cx_list_t *heapList;
    GLEXHeap *heap;
    int i;

    if (curr != context)
        glexMakeCurrent(context);

    glexShutdownProgram(context);
    
    for (i = 0; i < GLEX_HEAP_TYPE_MAX; ++i) {
        heapList = &context->heapLists[i];
        while ((node = cx_list_head(heapList)) != cx_list_knot(heapList)) {
            heap = CX_MEMBEROF(node, GLEXHeap, node);
            glexFreeHeap(heap);
        }
    }

    if (curr != context)
        glexMakeCurrent(curr);
    else
        glexMakeCurrent(NULL);

    free(context);
}

GLEX_API GLEXContext *glexCurrentContext(void)
{
    return glex;
}

GLEX_API void glexMakeCurrent(GLEXContext *context)
{
    glex = context;
}

#ifdef GLEX_LOG
GLEX_API void glexLogMask(GLuint mask)
{
	CX_ASSERT(glex != NULL);

	glex->logMask = mask;
}
#endif

GLEX_API void glexRenderMode(GLEXRenderMode mode)
{
	CX_ASSERT(glex != NULL);

	switch (mode) {
	case GLEX_RENDER_MODE_NONE:
		glex->renderFunc = glexRenderNone;
		GLEX_LOGD("Render mode: None");
		break;
	case GLEX_RENDER_MODE_FORWARD:
		glex->renderFunc = glexRenderForward;
		GLEX_LOGD("Render mode: Forward");
		break;
	case GLEX_RENDER_MODE_FORWARD_PLUS:
		glex->renderFunc = glexRenderForwardPlus;
		GLEX_LOGD("Render mode: ForwardPlus");
		break;
	case GLEX_RENDER_MODE_DEFERRED:
		glex->renderFunc = glexRenderDeferred;
		GLEX_LOGD("Render mode: Deferred");
		break;
	default:
		GLEX_LOGD("Render mode: Unknown");
		return;
	}

	glex->renderMode = mode;

}

GLEX_API void glexBeginFrame(void)
{
    CX_ASSERT(glex != NULL);

    glex->frame.meshCount = 0;
    glex->frame.lightCount = 0;
}

GLEX_API void glexEndFrame(void)
{
    CX_ASSERT(glex != NULL);

    if (glex->frame.meshCount < 1)
        return;

	CX_ASSERT(glex->renderFunc != NULL);
	glex->renderFunc();
}

GLEX_API void glexViewMatrix(const float *matrix4x4)
{
    CX_ASSERT(glex != NULL);

    memcpy(glex->viewMatrix.Elements, matrix4x4, sizeof(float) * 16);
}

GLEX_API void glexProjectionMatrix(const float *matrix4x4)
{
    CX_ASSERT(glex != NULL);

    memcpy(glex->projectionMatrix.Elements, matrix4x4, sizeof(float) * 16);
}

GLEX_API GLEXLight *glexCreateLight(void)
{
    return NULL;
}

GLEX_API void glexDeleteLight(GLEXLight *light)
{

}

GLEX_API void glexAddLight(GLEXLight *light, const float *transform)
{
    GLEXFrameLight *frameLight;

    CX_ASSERT(glex != NULL);
    CX_ASSERT(light != NULL);
    CX_ASSERT(transform != NULL);
    
    if (glex->frame.lightCount >= GLEX_FRAME_LIGHT_MAX)
        return;

    frameLight = &glex->frame.lightArray[glex->frame.lightCount++];

    frameLight->light = light;
    memcpy(frameLight->transform.Elements, transform, sizeof(float) * 16);
}

GLEX_API GLEXMaterial *glexCreateMaterial(void)
{
    return NULL;
}

GLEX_API void glexDeleteMaterial(GLEXMaterial *material)
{

}

GLEX_API GLEXMesh *glexCreateMesh(GLEXMeshType type, GLEXMeshMode mode,
	int vertexCount, int vertexIndexCount)
{
    GLEXMesh *mesh;
    int ret;
    GLenum usage;
	GLenum glMode;

    CX_ASSERT(glex != NULL);
    CX_ASSERT(vertexCount > 0 && vertexIndexCount > 0);

    switch (type) {
    case GLEX_MESH_TYPE_STATIC:
        usage = GL_STATIC_DRAW;
        break;
    case GLEX_MESH_TYPE_DYNAMIC:
        usage = GL_DYNAMIC_DRAW;
        break;
    default:
        goto bad0;
    }

	switch (mode) {
	case GLEX_MESH_MODE_POINTS:
		glMode = GL_POINTS;
		break;
	case GLEX_MESH_MODE_LINES:
		glMode = GL_LINES;
		break;
	case GLEX_MESH_MODE_TRIAGLEXES:
		glMode = GL_TRIANGLES;
		break;
	default:
		goto bad0;
	}

    mesh = malloc(sizeof(GLEXMesh));
    if (mesh == NULL)
        goto bad0;

    ret = glexAllocBuffer(&mesh->vertexBuffer, sizeof(GLEXVertex) * vertexCount, GL_ARRAY_BUFFER, usage);
    if (ret < 0)
        goto bad1;

    ret = glexAllocBuffer(&mesh->vertexIndexBuffer, sizeof(GLEXVertexIndex) * vertexIndexCount, GL_ELEMENT_ARRAY_BUFFER, usage);
    if (ret < 0)
        goto bad2;

    glGenVertexArrays(1, &mesh->vao);
    if (mesh->vao == 0)
        goto bad3;

    glBindVertexArray(mesh->vao);

    glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer.heap->id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->vertexIndexBuffer.heap->id);

	int pos = mesh->vertexBuffer.offset + CX_OFFSETOF(GLEXVertex, pos);
	//int normal = (void *)(mesh->vertexBuffer.offset + CX_OFFSETOF(GLEXVertex, normal));
	//int texCoord = (void *)(mesh->vertexBuffer.offset + CX_OFFSETOF(GLEXVertex, texCoord));

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLEXVertex), (const void *)(GLsizeiptr)pos);
    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLEXVertex), (const void *)normal);
    //glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GLEXVertex), (const void *)texCoord);

    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

	mesh->mode = glMode;
    mesh->vertexCount = vertexCount;
    mesh->vertexIndexCount = vertexIndexCount;

    return mesh;

bad3:
    glexFreeBuffer(&mesh->vertexIndexBuffer);

bad2:
    glexFreeBuffer(&mesh->vertexBuffer);

bad1:
    free(mesh);

bad0:
    return NULL;
}

GLEX_API void glexDeleteMesh(GLEXMesh *mesh)
{
    CX_ASSERT(mesh != NULL);
    CX_ASSERT(glex != NULL);

    glexFreeBuffer(&mesh->vertexBuffer);
    glexFreeBuffer(&mesh->vertexIndexBuffer);

    free(mesh);
}

GLEX_API void glexMeshVertexData(GLEXMesh *mesh, int offset, const GLEXVertex *p, int count)
{
    GLEXBuffer *buffer;
    int size;

    CX_ASSERT(glex != NULL);
    CX_ASSERT(mesh != NULL);
    CX_ASSERT(mesh->vertexCount > 0);

    buffer = &mesh->vertexBuffer;
    size = sizeof(GLEXVertex) * count;
    CX_ASSERT((offset + size) <= buffer->size);
    CX_ASSERT(buffer->heap->vbo > 0);

    glBindBuffer(GL_ARRAY_BUFFER, buffer->heap->id);
    glBufferSubData(GL_ARRAY_BUFFER, buffer->offset + (sizeof(GLEXVertex) * offset), size, p);
}

GLEX_API void glexMeshVertexIndexData(GLEXMesh *mesh, GLint offset, const GLEXVertexIndex *p, GLint count)
{
    GLEXBuffer *buffer;
    GLint size;

    CX_ASSERT(glex != NULL);
    CX_ASSERT(mesh != NULL);
    CX_ASSERT(mesh->vertexIndexCount > 0);

    buffer = &mesh->vertexIndexBuffer;
    size = sizeof(GLEXVertexIndex) * count;
    CX_ASSERT((offset + size) <= buffer->size);
    CX_ASSERT(buffer->heap->vbo > 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer->heap->id);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, buffer->offset + (sizeof(GLEXVertexIndex) * offset), size, p);
}

GLEX_API void glexAddMesh(GLEXMesh *mesh, GLEXMaterial *material, const GLfloat *transform)
{
    GLEXFrameMesh *frameMesh;

    CX_ASSERT(glex != NULL);
    CX_ASSERT(mesh != NULL);
    //CX_ASSERT(material != NULL);
    CX_ASSERT(transform != NULL);

    if (glex->frame.meshCount >= GLEX_FRAME_MESH_MAX)
        return;

    frameMesh = &glex->frame.meshArray[glex->frame.meshCount++];

    frameMesh->mesh = mesh;
    frameMesh->material = material;

    memcpy(frameMesh->transform.Elements, transform, sizeof(float) * 16);
}

