#pragma once

#include "ValveModule.hpp"
#include "actionCreators/ServoValveErrorMessage.hpp"
#include "../../system/ActionCreator.hpp"

ActionCreator* openValvesSynchronusly(
    ValveModule* valveModule,
    unsigned short firstValveId,
    unsigned short secondValveId,
    ActionCreator* nextTargetOnSuccess,
    ActionCreator* nextTargetOnFailture = nullptr,
    bool deleteSuccessTargetOnFailture = false,
    bool deleteFailtureTargetOnSuccess = false) {
        if (!valveModule->open(firstValveId)) {
            if (deleteSuccessTargetOnFailture && nextTargetOnSuccess) {
                delete nextTargetOnSuccess;
            }

            return servoValveErrorMessage(firstValveId);
        }

        if (!valveModule->open(secondValveId)) {
            if (deleteSuccessTargetOnFailture && nextTargetOnSuccess) {
                delete nextTargetOnSuccess;
            }

            if (!valveModule->close(firstValveId)) {
                return servoValveErrorMessage(firstValveId,
                    servoValveErrorMessage(secondValveId,
                        nextTargetOnFailture
                    )
                );
            }

            return servoValveErrorMessage(secondValveId, nextTargetOnFailture);
        }
        
        if (deleteFailtureTargetOnSuccess && nextTargetOnFailture) {
            delete nextTargetOnFailture;
        }

        return nextTargetOnSuccess;
}

ActionCreator* closeValvesSynchronusly(
    ValveModule* valveModule,
    unsigned short firstValveId,
    unsigned short secondValveId,
    ActionCreator* nextTargetOnSuccess,
    ActionCreator* nextTargetOnFailture = nullptr,
    bool deleteSuccessTargetOnFailture = false,
    bool deleteFailtureTargetOnSuccess = false) {
        InformationBannerActionCreator
            *firstValveError = nullptr,
            *secondValveError = nullptr;

        if (!valveModule->close(firstValveId)) {
            firstValveError = servoValveErrorMessage(firstValveId, nextTargetOnFailture);
        }

        if (!valveModule->close(secondValveId)) {
            secondValveError = servoValveErrorMessage(secondValveId, 
                firstValveError != nullptr ? firstValveError : nextTargetOnFailture
            );
        }

        if (firstValveError || secondValveError) {
            if (deleteSuccessTargetOnFailture && nextTargetOnSuccess) delete nextTargetOnSuccess;
        } else {
            if (deleteFailtureTargetOnSuccess && nextTargetOnFailture) delete nextTargetOnFailture;
        }

        return
            secondValveError != nullptr ? secondValveError :
            firstValveError != nullptr ? firstValveError :
            nextTargetOnSuccess;
}