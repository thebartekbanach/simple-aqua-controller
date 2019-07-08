import { applyMiddleware, combineReducers, createStore } from "redux";
import { composeWithDevTools } from "redux-devtools-extension";
import { SystemReducers } from "./system";
import { ISystemState } from "./system/types/systemState";

const reducers = {
    system: SystemReducers.systemReducer
};

export interface IApplicationState {
    system: ISystemState;
};

export function initializeStore(initialState?: any) {
    return createStore(combineReducers(reducers),
        initialState,
        composeWithDevTools(applyMiddleware())
    );
}
