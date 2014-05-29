#include "category.h"
#include <QDir>
#include <QDebug>

CategoryImage::CategoryImage()
{ }


QPixmap CategoryImage::getImage(QSize size) const {
    if (!cache.contains(size))
        cache.insert(size,
                     _image.scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    return cache[size];
}

QString CategoryImage::category()   const { return _category; }
QString CategoryImage::filename()   const { return _filename; }
QString CategoryImage::name()       const { return _name; }


bool CategoryImage::isNull() const { return _image.isNull(); }


CategoryImage::CategoryImagePtr CategoryImage::CreateFromFile(QString filename, QString category, QString name)
{
    qDebug() << "load image " << filename;

    auto image = CategoryImagePtr(new CategoryImage);
    image->_image = QPixmap(filename);
    image->_name = name;
    image->_filename = filename;
    image->_category = category;
    return image;
}


QPixmap CategoryImage::Thumbnail(QSize thumbnailSize) const {
    if (_thumbnail.isNull() || _thumbnail.size() != thumbnailSize) {
        _thumbnail = _image.scaled(thumbnailSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        qDebug() << "generate thumbnail for " << _name;
    }

    return _thumbnail;
}


/* ========================================== */


Category::Category()
{ }


const QVector<CategoryImage::CategoryImagePtr>  Category::listImages()  const { return _images; }
const CategoryImage::CategoryImagePtr           Category::getArt()      const { return _art; }
const QString                                   Category::Name()        const { return _name; }


Category::CategoryPtr Category::CreateFromDir(QString path) {
    qDebug() << "create category from " << path;
    auto dir = QDir(path);

    auto category = CategoryPtr(new Category);
    category->_name = dir.dirName();

    QStringList filters;
    filters << "*.jpg" << "*.png";
    dir.setNameFilters(filters);

    dir.setFilter(QDir::Files);
    dir.setSorting(QDir::Name);

    auto files = dir.entryInfoList(QDir::Files, QDir::Name);
    for (auto file = files.begin(); file != files.end(); ++file) {
        auto catImage = CategoryImage::CreateFromFile(file->absoluteFilePath(), category->Name(), file->baseName());
        if (file->baseName() == "Thumb")
            category->_art = catImage;
        else
            category->_images.push_back(catImage);
    }

    if (category->_art.isNull())
        category->_art = category->_images.first();

    return category;
}


/* ========================================== */

uint qHash(const QSize& s, uint seed) {
    return s.height() ^ s.width() ^ seed;
}
