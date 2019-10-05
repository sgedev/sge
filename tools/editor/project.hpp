//
//
#ifndef SGE_EDITOR_PROJECT_HPP
#define SGE_EDITOR_PROJECT_HPP

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <sge.hpp>

#include <QList>

class Project: public SGE::Game {
public:
	Project(void);
	virtual ~Project(void);

public:
	bool init(const char *root);
	void shutdown(void) override;
	bool handleEvent(const SGE::Event *evt) override;
	void update(float elapsed) override;
	void draw(SGE::View *v) override;
	bool save(const char *path);
	bool load(const char *path);
	bool importFile(const char *import_path, const char *filename);
	bool exportFile(const char *export_path, const char *filename);

protected:
	int trapFps(lua_State *L) override;
	int trapEditorIsEnabled(lua_State *L) override;

private:
	Assimp::Importer m_importer;
};

#endif // SGE_EDITOR_PROJECT_HPP
