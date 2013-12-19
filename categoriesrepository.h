#ifndef CATEGORIESREPOSITORY_H
#define CATEGORIESREPOSITORY_H

#include <QSharedPointer>
#include <QVector>
#include <category.h>

class CategoriesRepository
{
    CategoriesRepository();

    QVector<Category::CategoryPtr> _categories;
public:
    static CategoriesRepository *Instance();

    void loadCategoriesFromDir(QString root);
    const QVector<Category::CategoryPtr> listCategories() const;
};

#endif // CATEGORIESREPOSITORY_H
