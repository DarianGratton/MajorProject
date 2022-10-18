#include "Normalize.h"

namespace GameAgent
{
namespace DataManip
{

Normalize::Normalize(double p /* = 2.0 */, int64_t dim /* = 1 */, double eps /* = 1e-12 */) :
	p(p), dim(dim), eps(eps)
{

}

torch::Tensor Normalize::ModifyData(torch::Tensor data)
{
	auto options = torch::nn::functional::NormalizeFuncOptions().p(p).dim(dim).eps(eps);
	return torch::nn::functional::normalize(data, options);
}


} /* namespace DataManip */
} /* namespace GameAgent */
