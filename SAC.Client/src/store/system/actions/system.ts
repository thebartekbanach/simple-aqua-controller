import { createActionCreator } from "deox";

export const emergencyStop = createActionCreator("SYSTEM/EMERGENCY_STOP");
export const restartDriver = createActionCreator("SYSTEM/RESTART_DRIVER");
export const restartSystem = createActionCreator("SYSTEM/RESTART_SYSTEM");
export const turnOffSystem = createActionCreator("SYSTEM/TURN_OFF_SYSTEM");
export const connectionEstablished = createActionCreator("SYSTEM/CONNECTION_ESTABLISHED");
