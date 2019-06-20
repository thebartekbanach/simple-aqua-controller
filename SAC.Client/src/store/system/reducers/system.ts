import { createReducer } from "deox";

import {
    connectionEstablished,
    emergencyStop,
    restartDriver,
    restartSystem,
    turnOffSystem
} from "~/store/system/actions/system";

import { LastSystemState } from "../types/lastSystemState";
import { ISystemState } from "../types/systemState";

const defaultState: ISystemState = {
    connected: false,
    lastSystemState: LastSystemState.OK
};

export const systemReducer = createReducer(defaultState, (handleAction) => [
    handleAction(emergencyStop, (state) => ({ ...state, lastSystemState: LastSystemState.EMERGENCY_STOP })),
    handleAction(restartDriver, (state) => ({ ...state, lastSystemState: LastSystemState.DRIVER_RESTART })),
    handleAction(restartSystem, (state) => ({ ...state, lastSystemState: LastSystemState.SYSTEM_RESTART })),
    handleAction(turnOffSystem, (state) => ({ ...state, lastSystemState: LastSystemState.SYSTEM_OFF })),
    handleAction(connectionEstablished, (state) => ({ ...state, lastSystemState: LastSystemState.OK }))
]);
