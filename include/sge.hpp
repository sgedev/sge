//
//
#ifndef SGE_HPP
#define SGE_HPP

#include <cassert>

#include <sge.h>

namespace sge {
	class fs {
	public:
		inline fs(void)
			: m_fs(NULL)
		{
		}

		virtual ~fs(void)
		{
			if (m_fs != NULL)
				shutdown();
		}

	public:
		inline bool init(const char *zip_filename)
		{
			assert(m_fs == NULL);
			assert(zip_filename != NULL);

			m_fs = sge_fs_new(zip_filename);
			if (m_fs == NULL)
				return false;

			return true;
		}

		inline void shutdown(void)
		{
			assert(m_fs != NULL);

			sge_fs_destroy(m_fs);
			m_fs = NULL;
		}

		inline sge_fs_t *raw(void)
		{
			return m_fs;
		}

	private:
		sge_fs_t *m_fs;
	};

	class camera {
	public:
		inline camera(void)
			: m_camera(NULL)
		{
		}

		virtual ~camera(void)
		{
			if (m_camera != NULL)
				shutdown();
		}

	public:
		inline bool init(const sge_camera_type_t *type)
		{
			assert(type != NULL);
			assert(m_camera == NULL);

			m_camera = sge_camera_new(type);
			if (m_camera == NULL)
				return false;

			return true;
		}

		inline void shutdown(void)
		{
			assert(m_camera != NULL);

			sge_camera_destroy(m_camera);
			m_camera = NULL;
		}

		inline sge_camera_t *raw(void)
		{
			return m_camera;
		}

		inline void draw(void)
		{
			assert(m_camera != NULL);

			sge_camera_draw(m_camera);
		}

	private:
		sge_camera_t *m_camera;
	};

	class camera_opengl: public camera {
	public:
		inline camera_opengl(void)
		{
		}

		virtual ~camera_opengl(void)
		{
		}

	public:
		bool init(void)
		{
			return camera::init(SGE_CAMERA_TYPE_OPENGL);
		}
	};

	class camera_software: public camera {
	public:
		inline camera_software(void)
		{
		}

		virtual ~camera_software(void)
		{
		}

	public:
		bool init(void)
		{
			return camera::init(SGE_CAMERA_TYPE_SOFTWARE);
		}
	};

	class scene {
	public:
		inline scene(void)
			: m_scene(NULL)
		{
		}

		virtual ~scene(void)
		{
			if (m_scene != NULL)
				shutdown();
		}

	public:
		inline bool init(fs &root, const char *dirname)
		{
			assert(dirname != NULL);
			assert(m_scene == NULL);

			m_scene = sge_scene_new(root.raw(), dirname);
			if (m_scene == NULL)
				return false;

			return true;
		}

		inline void shutdown(void)
		{
			assert(m_scene != NULL);

			sge_scene_destroy(m_scene);
			m_scene = NULL;
		}

		inline sge_scene_t *raw(void)
		{
			return m_scene;
		}

		inline void update(float elapsed)
		{
			assert(m_scene != NULL);

			sge_scene_update(m_scene, elapsed);
		}

		inline void draw(camera *p)
		{
			assert(m_scene != NULL);

			sge_scene_draw(m_scene, p->raw());
		}

	private:
		sge_scene_t *m_scene;
	};
}

#endif // SGE_HPP

