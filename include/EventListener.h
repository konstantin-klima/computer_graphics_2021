//
// Created by rentorious on 09/02/2021.
//

#ifndef PROJECT_BASE_EVENTLISTENER_H
#define PROJECT_BASE_EVENTLISTENER_H

#include "../src/constants.h"
#include "reactphysics3d/reactphysics3d.h"
#include "../src/Entity/Component.h"
#include "../src/Entity/EntityManager.h"

class EventListener : public rp3d::EventListener {
    virtual void onContact(const rp3d::CollisionCallback::CallbackData &callbackData) override {
        for (uint p = 0; p < callbackData.getNbContactPairs(); p++) {
            rp3d::CollisionCallback::ContactPair contactPair = callbackData.getContactPair(p);

            auto b1 = contactPair.getBody1();
            auto b2 = contactPair.getBody2();
            auto c1 = contactPair.getCollider2();
            auto c2 = contactPair.getCollider2();


            std::string b1Name = (char *) b1->getUserData();
            std::string b2Name = (char *) b2->getUserData();

            if ((b1Name == "ARENA" || b1Name == "PLAYER1" || b1Name == "PLAYER2") &&
                (b2Name == "ARENA" || b2Name == "PLAYER1" || b2Name == "PLAYER2")) {
                auto pBody = b2Name == "ARENA" ? b1 : b2;
                auto pName = b2Name == "ARENA" ? b1Name : b2Name;

                auto players = EntityManager::getManager().getEntitiesWithComponent<CameraComponent>();
                auto res = std::find_if(players.begin(), players.end(), [pName](Entity *e) {
                    return (char *) e->getComponent<CollisionBodyComponent>()->getBody()->getUserData() == pName;
                });

                auto player = *res;

                auto eventType = contactPair.getEventType();

                if (eventType == rp3d::CollisionCallback::ContactPair::EventType::ContactExit) {
                    player->getComponent<CollisionBodyComponent>()->setIsFalling(true);
                    return;
                }
                player->getComponent<CollisionBodyComponent>()->setIsFalling(false);

                for (uint c = 0; c < contactPair.getNbContactPoints(); c++) {
                    auto contactPoint = contactPair.getContactPoint(c);

                    auto worldPoint1 = contactPair.getCollider1()->getLocalToWorldTransform() *
                                       contactPoint.getLocalPointOnCollider1();
                    auto worldPoint2 = contactPair.getCollider2()->getLocalToWorldTransform() *
                                       contactPoint.getLocalPointOnCollider2();
                    long double maxEps = 0.001;
                    auto worldDiff = worldPoint1 - worldPoint2;
                    if ((worldDiff.y < 0 && fabs(worldDiff.z) < maxEps && fabs(worldDiff.x) < maxEps)) {
                        continue;
                    }

                    auto pos = pBody->getTransform().getPosition();
                    auto ori = pBody->getTransform().getOrientation();

                    float factor = pBody == b1 ? -1.00001 : 1.00001;
                    pos += contactPoint.getWorldNormal() * contactPoint.getPenetrationDepth() * factor;

                    pBody->setTransform(rp3d::Transform(pos, ori));
                }
            }
        }
    }
};

#endif //PROJECT_BASE_EVENTLISTENER_H
