#include "collision.h"

HIT_DIRECTION CollisionComponent::CheckDirection(const CollisionComponent* collision) const
{
    if (!m_GameObject)return HIT_DIRECTION::NOT_HIT;
    //  Position一時コピー取って位置を調整
    XMINT2 ownerPosition = m_GameObject->GetPosition();
    const XMINT2& ownerScale = m_GameObject->GetScale();
    ownerPosition.x += static_cast<int>(ownerScale.x * 0.5f);
    ownerPosition.y += static_cast<int>(ownerScale.y * 0.5f);

    XMINT2 targetPosition = collision->m_GameObject->GetPosition();
    const XMINT2& targetScale = collision->m_GameObject->GetScale();
    targetPosition.x += static_cast<int>(targetScale.x*0.5f);
    targetPosition.y += static_cast<int>(targetScale.y*0.5f);

    //  ベクトル
    XMFLOAT2 vector = XMFLOAT2(static_cast<float>( ownerPosition.x - targetPosition.x),
        static_cast<float>(ownerPosition.y - targetPosition.y));

    //  ベクトル正規化
    XMVECTOR vecNormalize = XMLoadFloat2(&vector);
    vecNormalize =  XMVector2Normalize(vecNormalize);
    XMStoreFloat2(&vector, vecNormalize);

    //  ベクトルのxy成分で距離を探す
    if (std::abs(vector.x) > std::abs(vector.y)) 
    {
        if (vector.x > 0) 
        {
            return HIT_DIRECTION::RIGHT;
        }
        else
        {
            return HIT_DIRECTION::LEFT;
        }

    }
    else 
    {
        if (vector.y < 0) 
        {
            return HIT_DIRECTION::UP;
        }
        else 
        {
            return HIT_DIRECTION::DOWN;
        }
    }
    return HIT_DIRECTION::NOT_HIT;

}

HIT_DIRECTION CollisionComponent::CheckUpDirection(const CollisionComponent* collision) const
{
    if (!m_GameObject)return HIT_DIRECTION::NOT_HIT;
    //  Position一時保存
    const XMINT2& ownerPosition = m_GameObject->GetPosition();
    const XMINT2& targetPosition = collision->m_GameObject->GetPosition();

    //  ベクトル
    float vector = static_cast<float>(ownerPosition.y - targetPosition.y);

    //  上下の方向だけ確認
    if (vector < 0)
    {
        return HIT_DIRECTION::UP;
    }
    else 
    {
        return HIT_DIRECTION::DOWN;
    }
    return HIT_DIRECTION::NOT_HIT;
}

HIT_DIRECTION CollisionComponent::CheckRightDirection(const CollisionComponent* collision) const
{
    if (!m_GameObject)return HIT_DIRECTION::NOT_HIT;
    //  Position一時コピー取る
    XMINT2 ownerPosition = m_GameObject->GetPosition();
    ownerPosition.x += m_GameObject->GetScale().x;
    XMINT2 targetPosition = collision->m_GameObject->GetPosition();
    targetPosition.x += collision->m_GameObject->GetScale().x;


    //  ベクトル
    float vector = static_cast<float>(ownerPosition.x - targetPosition.x);

    //  左右の方向だけ確認
    if (vector > 0)
    {
        return HIT_DIRECTION::RIGHT;
    }
    else
    {
        return HIT_DIRECTION::LEFT;
    }

    return HIT_DIRECTION::NOT_HIT;
}

