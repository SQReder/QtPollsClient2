#ifndef CATEGORY_H
#define CATEGORY_H

#include <QPixmap>
#include <QVector>
#include <QSharedPointer>

class CategoryImage {
    CategoryImage();

    QPixmap _image;
    QString _name;
    QString _filename;
public:
    typedef QSharedPointer<CategoryImage> CategoryImagePtr;

    static CategoryImagePtr CreateFromFile(QString filename);

    QPixmap Thumbnail(QSize thumbnailSize) const;
    bool isNull() const;

    QPixmap getImage() const;
    QString filename() const;
    QString name() const;
};


class Category
{
    CategoryImage::CategoryImagePtr _art;
    QVector<CategoryImage::CategoryImagePtr> _images;
    QString _name;

    Category();

public:
    typedef QSharedPointer<Category> CategoryPtr;

    CategoryPtr CreateFromDir(QString path);

    const QVector<CategoryImage::CategoryImagePtr> listImages() const;
    const CategoryImage::CategoryImagePtr getArt() const;
    const QString Name() const;
};

#endif // CATEGORY_H
