#include "TorchDistribution.h"

#include <ctype.h>

vector<int64_t> TorchDistribution::extended_shape(c10::ArrayRef<int64_t> sample_shape)
{
    vector<int64_t> output_shape;

    output_shape.insert(output_shape.end(),
        sample_shape.begin(),
        sample_shape.end());
    output_shape.insert(output_shape.end(),
        batch_shape.begin(),
        batch_shape.end());
    output_shape.insert(output_shape.end(),
        event_shape.begin(),
        event_shape.end());
    
    return output_shape;
}
