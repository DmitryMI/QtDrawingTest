#ifndef SETUTILS_H
#define SETUTILS_H

#include <netparams.h>
#include <path.h>
#include <QSet>
#include <QVector>

typedef enum SetComparisonResult
{
    Less = -1,
    Equal = 0,
    Greater = 1,
    Uncomparable = 2
} SetComparisonResult;

class SetUtils
{
    SetUtils();
public:

    static bool IsSubset(QSet<int> *set1, QSet<int> *set2);

    static bool IsSubset(QVector<int> *set1, QVector<int> *set2);

    static SetComparisonResult CompareSets(QSet<int> *set1, QSet<int> *set2);

    static SetComparisonResult CompareSets(QVector<int> *setVector1, QVector<int> *setVector2);


    static SetComparisonResult ComparePaths(Path<NetParams> *path1, Path<NetParams> *path2);

    static void RemoveComplexPaths(QVector<Path<NetParams> *> *pathList);

    template <class T>
    static void CopyListContents(QVector<T> *source, QVector<T> *clone)
    {
        for(int i =0; i < source->length(); i++)
        {
            clone->append(source->at(i));
        }
    }

    static int SetIndexOf(QVector<int> *set, int value);

    static void SortedInsertUnique(QVector<int> *nodeIndexList, int value);

    static void FreeVectorMatrix(QVector<QVector<int>*> *vectorMatrix);
};

#endif // SETUTILS_H
