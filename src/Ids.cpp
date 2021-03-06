#define LOG_NAME "IDS"
#include "Lang.h"

#define IDS (*IdsArr)
#define NUM (*IdsNum)

static int idCap = INIT_IDS_NUM;

int AddId (Id **IdsArr, int *IdsNum, int64_t hash, char isConst, int len, int memOfs)
{
    Id new_id      = {};
    new_id.hash    = hash;
    new_id.isConst = isConst;
    new_id.len     = len;
    new_id.memOfs  = 1;

    if (NUM > 0)
    {
        new_id.memOfs = IDS[NUM - 1].memOfs + IDS[NUM - 1].len;
        LOG_MSG ("id added: %d; memOfs = %d\n", NUM, new_id.memOfs);
    }

    $ LOG_MSG ("\tnew hash = %ld; pos = %d\n", hash, NUM);

    IDS[NUM++] = new_id;

    if (NUM >= idCap)
    {
        $ Id *tmp = (Id *) realloc (IDS, (size_t) idCap * 2 * sizeof (Id));
        assert (tmp);

        IDS = tmp;
        idCap *= 2;
    }

    return NUM - 1;
}

int FindId (Id **IdsArr, int *IdsNum, int64_t hash, int reqOfs)
{
    for (int id = NUM - 1; id >= 0; id--)
    {
        if (IDS[id].hash == hash)
        {
            return id;
        }
    }

    return -1;
}

int RmId (Id **IdsArr, int *IdsNum, int num)
{
    if (NUM < 1) return 0;

    for (int id = 0; id < num; id++)
    {
        IDS[NUM--] = {};
    }

    if (NUM < idCap / 4)
    {
        $ Id *tmp = (Id *) realloc (IDS, (size_t) idCap / 2 * sizeof (Id));
        assert (tmp);

        IDS = tmp;
        idCap /= 2;
    }

    return NUM;
}
