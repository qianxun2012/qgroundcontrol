/****************************************************************************
 *
 * (c) 2021 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

#pragma once

#include <QObject>
#include <QString>
#include <QJsonDocument>

#include "ActuatorOutputs.h"
#include "ActuatorTesting.h"
#include "Mixer.h"
#include "GeometryImage.h"
#include "MotorAssignmentController.h"


class ActuatorsController : public QObject
{
    Q_OBJECT
public:
    ActuatorsController(Vehicle* vehicle);
    ~ActuatorsController() = default;

    Q_PROPERTY(QmlObjectListModel* actuatorOutputs         READ actuatorOutputs           NOTIFY actuatorOutputsChanged)
    Q_PROPERTY(QmlObjectListModel* actuatorActions         READ actuatorActions           NOTIFY actuatorActionsChanged)
    Q_PROPERTY(bool isMultirotor                           READ isMultirotor              CONSTANT)
    Q_PROPERTY(bool imageRefreshFlag                       READ imageRefreshFlag          NOTIFY imageRefreshFlagChanged)
    Q_PROPERTY(bool hasUnsetRequiredFunctions              READ hasUnsetRequiredFunctions NOTIFY hasUnsetRequiredFunctionsChanged)
    Q_PROPERTY(bool motorAssignmentActive                  READ motorAssignmentActive     NOTIFY motorAssignmentActiveChanged)
    Q_PROPERTY(QString motorAssignmentMessage              READ motorAssignmentMessage    NOTIFY motorAssignmentMessageChanged)

    Q_PROPERTY(ActuatorTesting::ActuatorTestController* actuatorTestController  READ actuatorTestController    CONSTANT)
    Q_PROPERTY(Mixer::MixerController* mixerController                          READ mixerController           CONSTANT)
    Q_PROPERTY(ActuatorOutputs::ActuatorOutput* selectedActuatorOutput          READ selectedActuatorOutput    NOTIFY selectedActuatorOutputChanged)

    Q_INVOKABLE void imageClicked(float x, float y);

    Q_INVOKABLE void selectActuatorOutput(int index);

    void load(const QString& json_file);

    QmlObjectListModel* actuatorOutputs() { return _actuatorOutputs; }
    ActuatorOutputs::ActuatorOutput* selectedActuatorOutput() const;

    ActuatorTesting::ActuatorTestController* actuatorTestController() { return &_actuatorTestController; }

    bool isMultirotor() const;

    bool imageRefreshFlag() const { return _imageRefreshFlag; }

    Mixer::MixerController* mixerController() { return &_mixerController; }

    bool hasUnsetRequiredFunctions() const { return _hasUnsetRequiredFunctions; }

    bool showUi() const;

    QmlObjectListModel* actuatorActions() { return _actuatorActions; }

    Q_INVOKABLE bool initMotorAssignment();
    Q_INVOKABLE void startMotorAssignment();
    Q_INVOKABLE void spinCurrentMotor() { _motorAssignmentController.spinCurrentMotor(); }
    Q_INVOKABLE void abortMotorAssignment();
    bool motorAssignmentActive() const { return _motorAssignmentController.active(); }
    const QString& motorAssignmentMessage() const { return _motorAssignmentController.message(); }

public slots:
    void parametersChanged();

signals:
    void actuatorOutputsChanged();
    void selectedActuatorOutputChanged();
    void imageRefreshFlagChanged();
    void hasUnsetRequiredFunctionsChanged();
    void motorAssignmentActiveChanged();
    void motorAssignmentMessageChanged();
    void actuatorActionsChanged();

private:
    void parseJson(const QJsonDocument& json);

    void updateAirframeImage();

    void updateActuatorActions();

    void subscribeFact(Fact* fact);

    Fact* getFact(const QString& paramName);

    void highlightActuators(bool highlight);

    QSet<Fact*> _subscribedFacts{};
    bool _hasMetadata{false};
    Condition _showUi;
    QmlObjectListModel* _actuatorOutputs = new QmlObjectListModel(this); ///< list of ActuatorOutputs::ActuatorOutput*
    QmlObjectListModel* _actuatorActions = new QmlObjectListModel(this); ///< list of ActuatorActionGroup*
    ActuatorTesting::ActuatorTestController _actuatorTestController;
    Mixer::MixerController _mixerController;
    MotorAssignmentController _motorAssignmentController;
    bool _hasUnsetRequiredFunctions{false};
    bool _imageRefreshFlag{false}; ///< indicator to QML to reload the image
    int _selectedActuatorOutput{0};
    Vehicle* _vehicle{nullptr};
};

