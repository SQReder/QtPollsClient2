#include "category.h"
#include <QDir>
#include <QDebug>

CategoryImage::CategoryImage()
{ }


QString CategoryImage::name()       const { return _name; }
QString CategoryImage::filename()   const { return _filename; }
QPixmap CategoryImage::getImage()   const { return _image; }


bool CategoryImage::isNull() const { return _image.isNull(); }


CategoryImage::CategoryImagePtr CategoryImage::CreateFromFile(QString filename)
{
    auto image = CategoryImagePtr(new CategoryImage);
    image->_image = QPixmap(filename);
    image->_name = filename;
    image->_filename = filename;
    return image;
}


QPixmap CategoryImage::Thumbnail(QSize thumbnailSize) const {
    static QPixmap thumbnail;
    
    if (thumbnail.isNull() || thumbnail.size() != thumbnailSize) {
        thumbnail = _image.scaled(thumbnailSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        qDebug() << "generate thumbnail for " << _name;
    }

    return thumbnail;
}


/* ========================================== */


Category::Category()
{ }


const QVector<CategoryImage::CategoryImagePtr>  Category::listImages()  const { return _images; }
const CategoryImage::CategoryImagePtr           Category::getArt()      const { return _art; }
const QString                                   Category::Name()        const { return _name; }


Category::CategoryPtr Category::CreateFromDir(QString path) {
    auto category = CategoryPtr(new Category);
    category->_name = path;

    auto dir = QDir(path);
    QStringList filters;
    filters << "*.jpg" << "*.png";
    dir.setNameFilters(filters);

    dir.setFilter(QDir::Files);
    dir.setSorting(QDir::Name);

    auto files = dir.entryInfoList(QDir::Files, QDir::Name);
    for (auto file = files.begin(); file != files.end(); ++file) {
        auto catImage = CategoryImage::CreateFromFile(file->absoluteFilePath());
        _images.push_back(catImage);
    }

    _art = _images.first();

    return category;
}


/* ========================================== */
