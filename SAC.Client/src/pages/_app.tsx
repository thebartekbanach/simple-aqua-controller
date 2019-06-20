import App, { Container } from "next/app";
import React from "react";
import { Provider } from "react-redux";
import { IContext, withReduxStore } from "~/lib/withReduxStore";

class Application extends App<IContext> {
    public render() {
        const { Component, pageProps, reduxStore } = this.props;
        return (
            <Container>
                <Provider store={reduxStore}>
                    <Component {...pageProps} />
                </Provider>
            </Container>
        );
    }
}

export default withReduxStore(Application);
