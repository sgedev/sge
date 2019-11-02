//
//
#ifndef SGE_RESOURCE_TEXTURE_HPP
#define SGE_RESOURCE_TEXTURE_HPP

#include <QObject>

#include <sge/resource/common.hpp>

SGE_RESOURCE_BEGIN

class Texture: public QObject {
	Q_OBJECT

public:
	Texture(QObject *parent = Q_NULLPTR);
	virtual ~Texture(void);
};

SGE_RESOURCE_END

#endif // SGE_RESOURCE_TEXTURE_HPP
