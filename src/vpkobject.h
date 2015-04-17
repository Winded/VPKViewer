#ifndef VPKOBJECT_H
#define VPKOBJECT_H

#include <QString>
#include "processor.h"

/// \brief Represents either a file or a folder inside a VPK package.
class VPKObject
{
		enum Type {
			File,
			Folder
		};

	public:
		QString path() const;

		QString name() const;

		Type type() const;

		VPKObject *parent() const;
		QList<VPKObject *> children() const;

		/// \brief Finds the VPKObject corresponding the given path
		/// \returns The VPKObject, if found
		static VPKObject *findObject(QString path);
		/// \brief Creates a VPKObject with the specified file
		/// \returns The newly created VPKObject, or null if it already existed
		static VPKObject *createObject(Processor::FileInfo fileInfo);
		static void clearObjects();

	private:
		VPKObject(QString path, Type type, int size = 0, VPKObject *parent = 0);

		VPKObject *mParent;
		Type mType;
		int mSize;
		QString mPath;
};

#endif // VPKOBJECT_H
