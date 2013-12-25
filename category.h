#ifndef CATEGORY_H
#define CATEGORY_H

#include <QPixmap>
#include <QVector>
#include <QHash>
#include <QSharedPointer>

uint qHash(const QSize& s, uint seed = 0);

class CategoryImage {
    CategoryImage();

    QPixmap _image;
    mutable QPixmap _thumbnail;
    QString _name;
    QString _filename;
    QString _category;
    mutable QHash<QSize, QPixmap> cache;

public:
    typedef QSharedPointer<CategoryImage> CategoryImagePtr;

    static CategoryImagePtr CreateFromFile(QString filename, QString category, QString name);

    QPixmap Thumbnail(QSize thumbnailSize) const;
    bool isNull() const;

    QPixmap getImage(QSize size) const;
    QString filename() const;
    QString name() const;
    QString category() const;
};


class Category
{
    CategoryImage::CategoryImagePtr _art;
    QVector<CategoryImage::CategoryImagePtr> _images;
    QString _name;

    Category();

public:
    typedef QSharedPointer<Category> CategoryPtr;

    static CategoryPtr CreateFromDir(QString path);

    const QVector<CategoryImage::CategoryImagePtr> listImages() const;
    const CategoryImage::CategoryImagePtr getArt() const;
    const QString Name() const;
};

#endif // CATEGORY_H
