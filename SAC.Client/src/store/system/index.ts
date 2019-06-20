import * as SystemActions from "./actions/system";
import * as SystemReducers from "./reducers/system";
import { ISystemState } from "./types/systemState";

export {
    SystemActions,
    SystemReducers
};

export type SystemState = ISystemState;
