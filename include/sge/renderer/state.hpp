//
//
#ifndef SGE_RENDERER_STATE_HPP
#define SGE_RENDERER_STATE_HPP

#include <QObject>

#include <sge/renderer/common.hpp>

SGE_RENDERER_BEGIN

class State: public QObject {
	Q_OBJECT

public:
	State(QObject *parent = Q_NULLPTR);
	virtual ~State(void);
};

SGE_RENDERER_END

#endif // SGE_RENDERER_STATE_HPP

