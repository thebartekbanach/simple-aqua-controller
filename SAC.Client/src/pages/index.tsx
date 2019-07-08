import Button from "@material-ui/core/Button";
import React from "react";
import { connect } from "react-redux";
import { Dispatch } from "redux";
import { IApplicationState } from "~/store";
import { SystemActions } from "~/store/system";
import { LastSystemState } from "~/store/system/types/lastSystemState";

interface IStateFromProps {
    lastState: LastSystemState;
}

interface IDispatchFromProps {
    connectionEstablished: () => void;
    emergencyStop: () => void;
    restartDriver: () => void;
    restartSystem: () => void;
    turnOffSystem: () => void;
}

type IndexPageProps = IStateFromProps & IDispatchFromProps;

class IndexPage extends React.Component<IndexPageProps> {
    public render() {
        return (
            <div>
                <p>Last action was: {this.getLastActionName()}</p>
                <p>System controls: </p>
                <p>
                    <Button variant="outlined" color="primary" onClick={this.props.connectionEstablished}>OK!</Button>
                    <Button variant="contained" color="primary" onClick={this.props.emergencyStop}>EMERGENCY_STOP</Button>
                    <Button variant="text" color="secondary" onClick={this.props.restartSystem}>SYSTEM_RESTART</Button>
                    <Button variant="text" color="secondary" onClick={this.props.restartDriver}>DRIVER_RESTART</Button>
                    <Button variant="text" color="primary" onClick={this.props.turnOffSystem}>SYSTEM_OFF</Button>
                </p>
            </div>
        );
    }

    private lastStateIs(type: LastSystemState): boolean {
        return this.props.lastState === type;
    }

    private getLastActionName() {
        return (
            this.lastStateIs(LastSystemState.EMERGENCY_STOP) ? "EMERGENCY_STOP" :
            this.lastStateIs(LastSystemState.DRIVER_RESTART) ? "DRIVER_RESTART" :
            this.lastStateIs(LastSystemState.SYSTEM_RESTART) ? "SYSTEM_RESTART" :
            this.lastStateIs(LastSystemState.SYSTEM_OFF) ? "SYSTEM_OFF" :
            "OK!"
        );
    }
}

const mapStateToProps = (state: IApplicationState): IStateFromProps => {
    return {
        lastState: state.system.lastSystemState
    };
};

const mapDispatchToProps = (dispatch: Dispatch): IDispatchFromProps => ({
    connectionEstablished: () => dispatch(SystemActions.connectionEstablished()),
    emergencyStop: () => dispatch(SystemActions.emergencyStop()),
    restartDriver: () => dispatch(SystemActions.restartDriver()),
    restartSystem: () => dispatch(SystemActions.restartSystem()),
    turnOffSystem: () => dispatch(SystemActions.turnOffSystem())
});

export default connect<IStateFromProps, IDispatchFromProps, IStateFromProps, IApplicationState>(
    mapStateToProps,
    mapDispatchToProps
)(IndexPage);
