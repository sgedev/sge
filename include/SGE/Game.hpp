//
//
#ifndef SGE_GAME_HPP
#define SGE_GAME_HPP

#include <QString>
#include <QElapsedTimer>
#include <QSharedPointer>
#include <QAbstractItemModel>
#include <QJSEngine>

#include <SGE/Common.hpp>
#include <SGE/ResourceManager.hpp>
#include <SGE/Object.hpp>
#include <SGE/VM.hpp>

SGE_BEGIN

typedef QSharedPointer<Object> ObjectPtr;
typedef QList<ObjectPtr> ObjectPtrList;

class Q_DECL_EXPORT Game: public QAbstractItemModel, public VM::TrapHandler {
	Q_OBJECT

public:
	Game(QObject *parent = Q_NULLPTR);
	virtual ~Game(void);

public:
	virtual bool init(const QString &baseDir);
	virtual void update(float elapsed);
	ResourceManager &resourceManager(void);
	int	columnCount(const QModelIndex & parent) const Q_DECL_OVERRIDE;
	QVariant data(const QModelIndex & index, int role) const Q_DECL_OVERRIDE;
	QModelIndex index(int row, int column, const QModelIndex & parent) const Q_DECL_OVERRIDE;
	QModelIndex	parent(const QModelIndex & index) const Q_DECL_OVERRIDE;
	int	rowCount(const QModelIndex & parent) const Q_DECL_OVERRIDE;
	ObjectID objectNew(const QString &name, ObjectID parent) Q_DECL_OVERRIDE;
	void objectDestroy(ObjectID obj) Q_DECL_OVERRIDE;

private:
	ResourceManager m_resourceManager;
};

SGE_INLINE ResourceManager &Game::resourceManager(void)
{
	return m_resourceManager;
}

SGE_END

#endif // SGE_GAME_HPP
