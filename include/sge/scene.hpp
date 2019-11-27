//
//
#ifndef SGE_SCENE_HPP
#define SGE_SCENE_HPP

#include <QObject>

#include <sge/common.hpp>
#include <sge/database.hpp>
#include <sge/renderer.hpp>

SGE_BEGIN

class Scene: public QObject {
	Q_OBJECT

public:
	Scene(QObject *parent = Q_NULLPTR);
	virtual ~Scene(void);

public:
	bool init(void);
	void shutdown(void);
	void update(float elapsed);
	void draw(Renderer::Context *rc);

private:
	Database::Group m_group;
};

SGE_END

#endif // SGE_SCENE_HPP

