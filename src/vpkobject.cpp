#include "vpkobject.h"
#include <QStringList>
#include <cassert>

namespace {
	QList<VPKObject*> objectList;
}

VPKObject::VPKObject(QString path, Type type, int size, VPKObject *parent) :
	mPath(path),
	mType(type),
	mSize(size),
	mParent(parent)
{
}

QString VPKObject::path() const {
	return mType == VPKObject::Folder ? mPath + "/" : mPath;
}

QString VPKObject::name() const {
	return mPath.split("/").last();
}

VPKObject::Type VPKObject::type() const {
	return mType;
}

VPKObject *VPKObject::parent() const {
	return mParent;
}

QList<VPKObject *> VPKObject::children() const {
	QList<VPKObject *> list;
	for(VPKObject *obj : objectList) {
		if(obj->parent() == this) {
			list.append(obj);
		}
	}
	return list;
}

VPKObject *VPKObject::findObject(QString path) {
	for(VPKObject *obj : objectList) {
		if(obj->path() == path) {
			return obj;
		}
	}
	return 0;
}

VPKObject *VPKObject::createObject(Processor::FileInfo fileInfo) {
	QString path = fileInfo.mPath;
	QStringList splitted = path.split("/", QString::SkipEmptyParts);
	VPKObject *parent = 0;
	for(int i = 0; i < splitted.size(); i++) {
		QString s = splitted.at(i);
		assert(s.isEmpty());
		QString cPath;
		for(int i2 = 0; i2 < i; i2++) {
			cPath += splitted.at(i2) + "/";
		}
		if(i == splitted.size() - 1) {
			if(VPKObject::findObject(cPath + s)) {
				return 0;
			}
			break;
		}
		else {
			VPKObject *p = VPKObject::findObject(cPath + s);
			if(!p) {
				p = new VPKObject(cPath + s, VPKObject::Folder, 0, parent);
				objectList.append(p);
			}
			parent = p;
		}
	}
	VPKObject *obj = new VPKObject(path, VPKObject::File, fileInfo.mSize, parent);
	objectList.append(obj);
	return obj;
}

void VPKObject::clearObjects() {
	qDeleteAll(objectList);
	objectList.clear();
}
