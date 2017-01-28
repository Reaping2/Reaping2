#pragma once
#ifndef INCLUDED_COUNTER_H
#define INCLUDED_COUNTER_H

#include <functional>
#include <vector>
#include "platform/rstdint.h"

namespace render {
struct RenderBatch
{
    int32_t TexId;
    int32_t MaskId;
    int32_t NormalId;
    int32_t SecondaryTexId;
    int32_t ShaderId;
    bool operator==(RenderBatch const& o) const
    {
        return TexId == o.TexId
            && MaskId == o.MaskId
            && NormalId == o.NormalId
            && SecondaryTexId == o.SecondaryTexId
            && ShaderId == o.ShaderId;
    }
    bool operator!=(RenderBatch const& o) const
    {
        return !operator==(o);
    }
};
struct CountByTexId
{
    RenderBatch Batch;
    size_t Start;
    size_t Count;
    CountByTexId() = default;
    CountByTexId( RenderBatch const& b, size_t s, size_t c ) : Batch( b ), Start( s ), Count( c ) {}
};
typedef std::vector<CountByTexId> Counts_t;
typedef std::function<bool( RenderBatch& )> GeneratorFunc;
Counts_t count( GeneratorFunc f );
}

#endif // INCLUDED_COUNTER_H

