#include "Model.h"

namespace nu
{
    void Model::CalculateRadious()
    {
        m_radious = 0.0f;
            for (const auto& mesh : m_meshes)
            {
                //get mesh radious
                float radious = mesh.GetRadious();
                //If radious is larger than current radious,radios is new radius
                    if (radious > m_radious)
                    {
                        m_radious = radious;
                    }
            }
     }


}