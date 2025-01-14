/*
 * Copyright 2021 Clearpath Robotics, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * @author Roni Kreinin (rkreinin@clearpathrobotics.com)
 */

#ifndef TURTLEBOT4_IGNITION_GUI_PLUGINS__TURTLEBOT4HMI__TURTLEBOT4HMI_HH_
#define TURTLEBOT4_IGNITION_GUI_PLUGINS__TURTLEBOT4HMI__TURTLEBOT4HMI_HH_

#include <ignition/transport/Node.hh>

#include <ignition/gui/qt.h>
#include <ignition/gui/Plugin.hh>

#include <string>

namespace ignition
{

namespace gui
{

class Turtlebot4Hmi : public Plugin
{
  Q_OBJECT

  // \brief Namespace
  Q_PROPERTY(
    QString namespace
    READ Namespace
    WRITE SetNamespace
    NOTIFY NamespaceChanged
  )

public:
  /// \brief Constructor
  Turtlebot4Hmi();
  /// \brief Destructor
  virtual ~Turtlebot4Hmi();
  /// \brief Called by Ignition GUI when plugin is instantiated.
  /// \param[in] _pluginElem XML configuration for this plugin.
  void LoadConfig(const tinyxml2::XMLElement * _pluginElem) override;
  // \brief Get the namespace as a string, for example
  /// '/echo'
  /// \return Namespace
  Q_INVOKABLE QString Namespace() const;

public slots: 
  /// \brief Callback in Qt thread when the namespace changes.
  /// \param[in] _namespace variable to indicate the namespace to
  /// publish the Button commands and subscribe to robot status.
  void SetNamespace(const QString &_namespace);

signals:
  /// \brief Notify that namespace has changed
  void NamespaceChanged();

protected slots:
  /// \brief Callback trigged when the button is pressed.
  void OnHmiButton(const int button);
  void OnCreate3Button(const int button);

  /// \brief QML signals
signals:
  void setPowerState(const bool state, const QString color);
  void setMotorsState(const bool state, const QString color);
  void setCommsState(const bool state, const QString color);
  void setWifiState(const bool state, const QString color);
  void setBatteryState(const bool state, const QString color);
  void setUser1State(const bool state, const QString color);
  void setUser2State(const bool state, const QString color);

  /// \brief Subscriber callbacks
private:
  void OnRawMessage(const ignition::msgs::StringMsg & msg);
  void OnSelectedMessage(const ignition::msgs::Int32 & msg);
  void OnPowerLedMessage(const ignition::msgs::Int32 & msg);
  void OnMotorsLedMessage(const ignition::msgs::Int32 & msg);
  void OnCommsLedMessage(const ignition::msgs::Int32 & msg);
  void OnWifiLedMessage(const ignition::msgs::Int32 & msg);
  void OnBatteryLedMessage(const ignition::msgs::Int32 & msg);
  void OnUser1LedMessage(const ignition::msgs::Int32 & msg);
  void OnUser2LedMessage(const ignition::msgs::Int32 & msg);
  void UpdateTopics();

signals:
  void AddMsg(QString msg);

private slots:
  void OnAddMsg(QString msg);

private:
  ignition::transport::Node node_;
  
  ignition::transport::Node::Publisher hmi_button_pub_;
  ignition::transport::Node::Publisher create3_button_pub_;

  std::string namespace_ = "turtlebot4";

  std::string hmi_button_topic_ = "/model/turtlebot4/hmi/buttons";
  std::string create3_button_topic_ = "/model/turtlebot4/buttons";

  std::string display_topic_ = "/model/turtlebot4/hmi/display/raw";
  std::string display_selected_topic_ = "/model/turtlebot4/hmi/display/selected";
  std::string power_led_topic_ = "/model/turtlebot4/hmi/led/power";
  std::string motors_led_topic_ = "/model/turtlebot4/hmi/led/motors";
  std::string comms_led_topic_ = "/model/turtlebot4/hmi/led/comms";
  std::string wifi_led_topic_ = "/model/turtlebot4/hmi/led/wifi";
  std::string battery_led_topic_ = "/model/turtlebot4/hmi/led/battery";
  std::string user1_led_topic_ = "/model/turtlebot4/hmi/led/user1";
  std::string user2_led_topic_ = "/model/turtlebot4/hmi/led/user2";

  QStringListModel display_list_;

  const unsigned int num_lines_{6u};
  const unsigned int char_per_line_{18u};
  const unsigned int char_per_line_header_{21u};
  int selected_line_{0u};

  std::mutex raw_msg_mutex_;
  std::mutex selected_msg_mutex_;
};

}  // namespace gui

}  // namespace ignition

#endif  // TURTLEBOT4_IGNITION_GUI_PLUGINS__TURTLEBOT4HMI__TURTLEBOT4HMI_HH_
