#include "ACtor.h"
#include "Vector2.h"
#include "Transformer.h"
#include "Renderer.h"
#include "Math_Util.h"
#include "Model.h"
namespace nu
{
	void Actor::Update(float dt)
		{
			m_transform.position += (m_velocity * dt);
			m_velocity *= (1.0f / (1.0f + m_damping * dt));
			if(m_tag!="PlayerBullet"){
			m_transform.position.x = Wrap(0.0f, 1920.0f, m_transform.position.x);
			m_transform.position.y = Wrap(0.0f, 1024.0f, m_transform.position.y);
			}
		}
	void Actor::Draw(const Renderer& renderer) const 
	{
		renderer.SetColor(0,0,0);
		renderer.DrawModel(m_model, m_transform);

		
		renderer.SetColor(255, 255, 255);
		//renderer.DrawFillRect(m_transform.position.x 
		//	- (m_transform.scale * 0.5f),
		//	m_transform.position.y, m_transform.scale,m_transform.scale);
	
	}

	float Actor::GetRadious() const
	{
		return m_model.GetRadious() * m_transform.scale * 0.5f;
	}




}