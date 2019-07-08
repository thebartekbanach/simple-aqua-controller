import { NextContext } from "next";
import { NextAppContext } from "next/app";
import { Component } from "react";
import { Store } from "redux";
import { initializeStore } from "~/store";
import { isServer } from "./isServer";

const __NEXT_REDUX_STORE__ = "__NEXT_REDUX_STORE__";

function getOrCreateStore(initialState?: any): Store {
    if (isServer) {
        return initializeStore(initialState);
    }

    const windowObject: any = window;

    if (!windowObject[__NEXT_REDUX_STORE__]) {
        windowObject[__NEXT_REDUX_STORE__] = initializeStore(initialState);
    }

    return windowObject[__NEXT_REDUX_STORE__];
}

export interface IContext extends NextContext {
    reduxStore: any;
}

interface IAppContext extends NextAppContext {
    ctx: IContext;
}

interface IAppWithReduxStore {
    initialReduxState: Store;
}

export const withReduxStore = (App: any) => {
    return class AppWithRedux extends Component<IAppWithReduxStore> {
        public static async getInitialProps(appContext: IAppContext) {
            const reduxStore = getOrCreateStore();

            appContext.ctx.reduxStore = reduxStore;

            let appProps = {};

            if (typeof (App as any).getInitialProps === "function") {
                appProps = await (App as any).getInitialProps(appContext);
            }

            return {
                ...appProps,
                initialReduxState: reduxStore.getState()
            };
        }

        private reduxStore: Store;

        constructor(props: IAppWithReduxStore) {
            super(props);

            this.reduxStore = getOrCreateStore(props.initialReduxState);
        }

        public render() {
            return <App {...this.props} reduxStore={this.reduxStore} />;
        }
    };
};
