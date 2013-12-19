#ifndef CATEGORIESREPOSITORY_H
#define CATEGORIESREPOSITORY_H

#include <QSharedPointer>
#include <QVector>
#include <category.h>

class CategoriesRepository
{
    CategoriesRepository();

    QVector<Category> _categories;
public:
    static QSharedPointer<CategoriesRepository> Instance();

    void loadCategoriesFromDir(QString root);
};

#endif // CATEGORIESREPOSITORY_H
