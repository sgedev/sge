//
//
#ifndef SGE_LEVEL_HPP
#define SGE_LEVEL_HPP

#include <QList>
#include <QSharedPointer>

#include <sge/common.hpp>
#include <sge/Scene.hpp>

SGE_BEGIN

typedef QSharedPointer<Scene> ScenePtr;
typedef ScenePtr<ScenePtr> SceneList;

class Level {
public:
	Level(void);
	virtual ~Level(void);

private:
	SceneList m_scenes;
};

SGE_END

#endif // SGE_LEVEL_HPP
