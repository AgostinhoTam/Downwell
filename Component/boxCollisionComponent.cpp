#include "boxCollisionComponent.h"

void BoxCollisionComponent::Init()
{
    UpdateCollision();
}

void BoxCollisionComponent::Update()
{
    UpdateCollision();
}

void BoxCollisionComponent::Draw(const int cameraOffsetY)
{
#if _DEBUG
    DrawBox(min.x, (min.y - cameraOffsetY), 
            max.x, (max.y - cameraOffsetY), 
            GetColor(255, 1, 1), FALSE);
#endif
}

bool BoxCollisionComponent::IsOverlapping(const CollisionComponent* collision) const
{
    if (!collision)return false;
    return collision->IsOverlapping(this);
}

bool BoxCollisionComponent::IsCollisionHit(const CollisionComponent* collision) const
{
    if (!collision)return false;
    return collision->IsCollisionHit(this);
}

bool BoxCollisionComponent::IsCollisionHit(const BoxCollisionComponent* collision) const
{
    if (collision)
    {
        return(max.x >= collision->min.x &&
            min.x <= collision->max.x &&
            max.y >= collision->min.y &&
            min.y <= collision->max.y);
    }
    return false;
}

bool BoxCollisionComponent::IsOverlapping(const BoxCollisionComponent* collision) const
{
    if (collision) 
    {
        return(max.x > collision->min.x &&
            min.x < collision->max.x &&
            max.y > collision->min.y &&
            min.y < collision->max.y);
    }
    return false;
}

void BoxCollisionComponent::UpdateCollision()
{
    if (!m_GameObject)return;
    const XMINT2& ownerPosition = m_GameObject->GetPosition();

    min.x = ownerPosition.x + m_offset.x;
    max.x = ownerPosition.x + m_size.x + m_offset.x;
    min.y = ownerPosition.y + m_offset.y;
    max.y = ownerPosition.y + m_size.y + m_offset.y;
}

