#include "setutils.h"

bool SetUtils::IsSubset(QSet<int> *set1, QSet<int> *set2)
{
    QList<int> values1 = set1->values();
    for(int i = 0; i < values1.count(); i++)
    {
        int value = values1.at(i);
        if(!set2->contains(value))
        {
            return false;
            break;
        }
    }

    return true;
}

bool SetUtils::IsSubset(QVector<int> *set1, QVector<int> *set2)
{
    for(int i = 0; i < set1->count(); i++)
    {
        int value = set1->at(i);
        if(!set2->contains(value))
        {
            return false;
            break;
        }
    }

    return true;
}

SetComparisonResult SetUtils::CompareSets(QSet<int> *set1, QSet<int> *set2)
{
    bool isSubset12 = IsSubset(set1, set2);

    bool isSubset21 = IsSubset(set2, set1);


    if(isSubset12)
    {
        return SetComparisonResult::Less;
    }
    else if(isSubset12 && isSubset21)
    {
        return SetComparisonResult::Equal;
    }
    else if(isSubset21)
    {
        return SetComparisonResult::Greater;
    }
    else
    {
        return SetComparisonResult::Uncomparable;
    }
}

SetComparisonResult SetUtils::CompareSets(QVector<int> *setVector1, QVector<int> *setVector2)
{
    QSet<int> set1 = QSet<int>();
    QSet<int> set2 = QSet<int>();

    for(int i = 0; i < setVector1->length(); i++)
    {
        set1.insert(setVector1->at(i));
    }
    for(int i = 0; i < setVector2->length(); i++)
    {
        set2.insert(setVector2->at(i));
    }

    return CompareSets(&set1, &set2);
}

int PathIndexOf( Path<NetParams> *path, int start, int end, int key)
{
    for(int j = start; j < end; j++)
    {
        auto node2 = path->at(j);
        int key2 = node2->GetKey();
        if(key == key2)
        {
           return j;
        }
    }

    return -1;
}

bool IsPathSubset(Path<NetParams> *path1, Path<NetParams> *possibleSubset)
{
    for(int i = 1; i < path1->length(); i++)
    {
        auto node1 = path1->at(i);
        int key1 = node1->GetKey();

        int indexOf = PathIndexOf(possibleSubset, 1, possibleSubset->length(), key1);

        if(indexOf == -1)
        {
            return false;
        }
    }

    return true;
}

SetComparisonResult SetUtils::ComparePaths(Path<NetParams> *path1, Path<NetParams> *path2)
{
    QSet<int> set1 = QSet<int>();
    QSet<int> set2 = QSet<int>();

    for(int i = 1; i < path1->length() - 1; i++)
    {
        set1.insert(path1->at(i)->GetKey());
    }
    for(int i = 1; i < path2->length() - 1; i++)
    {
        set2.insert(path2->at(i)->GetKey());
    }

    return CompareSets(&set1, &set2);
/*
    bool subset12 = IsPathSubset(path1, path2);
    bool subset21 = IsPathSubset(path2, path1);

    if(subset12 && subset21)
    {
        return SetComparisonResult::Equal;
    }

    if(subset12 && !subset21)
    {
        return Greater;
    }

    if(!subset12 && subset21)
    {
        return SetComparisonResult::Less;
    }

    if(!subset12 && !subset21)
    {
        return SetComparisonResult::Uncomparable;
    }

    return SetComparisonResult::Uncomparable;

    */
}

void SetUtils::RemoveComplexPaths(QVector<Path<NetParams> *> *pathList)
{
    for(int i = 0; i < pathList->count(); i++)
    {
        Path<NetParams> *current = pathList->at(i);
        for(int j = i + 1; j < pathList->count(); j++)
        {
            Path<NetParams> *comparable = pathList->at(j);
            SetComparisonResult comparisonResult = ComparePaths(current, comparable);

            if(comparisonResult == SetComparisonResult::Less)
            {
                pathList->removeAt(j);
                j--;
            }
            else if(comparisonResult == SetComparisonResult::Equal)
            {
                // This must not happen!
                pathList->removeAt(i);
                i--;
                break;
            }
            else if(comparisonResult == SetComparisonResult::Greater)
            {
                pathList->removeAt(i);
                i--;
                break;
            }
            else
            {
                // Uncomparible, do nothing
            }
        }
    }
}

int SetUtils::SetIndexOf(QVector<int> *set, int value)
{
    for(int i = 0; i < set->length(); i++)
    {
        int setAt = set->at(i);
        if(setAt == value || setAt == -value)
        {
            return i;
        }
    }

    return -1;
}

void SetUtils::SortedInsertUnique(QVector<int> *nodeIndexList, int value)
{
    if(nodeIndexList->length() == 0)
    {
        nodeIndexList->append(value);
        return;
    }

    int index = 0;
    while(index < nodeIndexList->length())
    {
        int valueAt = nodeIndexList->at(index);
        int comparison = value - valueAt;
        if(comparison > 0)
        {
            index++;
        }
        else if(comparison < 0)
        {
            break;
        }
        else
        {
            return;
        }
    }
    nodeIndexList->insert(index, value);
}

void SetUtils::FreeVectorMatrix(QVector<QVector<int> *> *vectorMatrix)
{
    for(int i = 0; i < vectorMatrix->length(); i++)
    {
        delete (vectorMatrix->at(i));
    }

    delete vectorMatrix;
}
