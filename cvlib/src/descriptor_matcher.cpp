/* Descriptor matcher algorithm implementation.
 * @file
 * @date 2018-11-25
 * @author Anonymous
 */

#include "cvlib.hpp"

namespace cvlib
{
void descriptor_matcher::knnMatchImpl(cv::InputArray queryDescriptors, std::vector<std::vector<cv::DMatch>>& matches, int k /*unhandled*/,
                                      cv::InputArrayOfArrays masks /*unhandled*/, bool compactResult /*unhandled*/)
{
}

void descriptor_matcher::radiusMatchImpl(cv::InputArray queryDescriptors, std::vector<std::vector<cv::DMatch>>& matches, float maxDistance,
                                         cv::InputArrayOfArrays masks /*unhandled*/, bool compactResult /*unhandled*/)
{
    if (trainDescCollection.empty())
        return;

    auto q_desc = queryDescriptors.getMat();
    auto& t_desc = trainDescCollection[0];

    matches.resize(q_desc.rows);

    bool flags[t_desc.rows] = {false};

    for (int i = 0; i < q_desc.rows; ++i)
    {
        int k1 = 0;
        int k2 = 0;
        float dist1 = maxDistance;
        float dist2 = maxDistance;
        for (int j = 0; j < t_desc.rows; ++j)
        {
            if (flags[j])
                continue;

            float norm = cv::norm(q_desc.row(i) - t_desc.row(j));

            if (!flags[j] && norm < dist1)
            {
                    dist1 = norm;
                    k1 = j;
            }
            else
                if (!flags[j] && norm < dist2)
                {
                    dist2 = norm;
                    k2 = j;
                }
        }

        if (dist1 / dist2 < 0.5 && k1 != k2 && k1 != 0 && k2 != 0)
        {
            matches[i].emplace_back(i, k1, dist1);
            flags[k1] = true;
        }
    }
}
} // namespace cvlib
