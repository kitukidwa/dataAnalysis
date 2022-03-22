/**
  (C) 2018 Copyright BARCO NV

  All rights reserved. Reproduction in whole or in part is
  prohibited without the written consent of the copyright owner.

  \file   mock_display_data.cpp
  \author ABSR
  \date   10/08/2018
  \brief  Implementation of the mock display data class which is creating response from dummy data
*/

// System Header
#include <variant>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
// Module headers
#include "mock_display_data.hpp"
#include "mock_test_contant.hpp"
#include "dramp_constants.hpp"
// Framework Header
#include "COBRACoreLogging/logger.hpp"

MockDisplayData::MockDisplayData() {
  ReadMockDataBase();
}

MockDisplayData::~MockDisplayData() {
}

std::string MockDisplayData::HandleGETRequest(std::string& req_url) {
  nlohmann::json display_data;
  try {
    // std::cout << "***" << req_url << std::endl;
  if (req_url == cobra::drampconstants::res_path_isalive) {
    std::string isAliveResp = std::string("{"
        "\"resource\" : {"
        "   \"name\" : \"ws://localhost:8080/dramp/2/data/isAlive/\""
        "}"
    "}");
    return isAliveResp;
  } else {
    return CreateGETCommandResponse(req_url);
  }
  } catch (std::exception &e) {
    std::cout << "exception: " + std::string(e.what());
  }
  return display_data.dump();
}

std::string MockDisplayData::HandlePOSTRequest(std::string& request, const std::string& msgid,
                                                bool sendInProgressOnly) {
  nlohmann::json postData = nlohmann::json::parse(request);
  std::string cmd = postData[cobra::drampconstants::action][cobra::drampconstants::name];

  if (cmd == cobra::drampconstants::update_operation_state) {
    std::string val = postData[cobra::drampconstants::params][0][cobra::drampconstants::value];
    return CreatePOSTCommandResponse("/data/device/operationstate/value", cmd, val, msgid, sendInProgressOnly);
  } else if (cmd == cobra::drampconstants::update_input) {
    std::string prop = postData[cobra::drampconstants::params][0][cobra::drampconstants::name];
    if (prop == cobra::drampconstants::param_selected_input) {
      std::string val = postData[cobra::drampconstants::params][0][cobra::drampconstants::value];
      return CreatePOSTCommandResponse("/data/input/selectedInput/value", cmd, val, msgid, sendInProgressOnly);
    } else if (prop == cobra::drampconstants::param_input_mode) {
      std::string val = postData[cobra::drampconstants::params][0][cobra::drampconstants::value];
      return CreatePOSTCommandResponse("/data/input/inputMode/value", cmd, val, msgid, sendInProgressOnly);
    }
  } else if (cmd == cobra::drampconstants::update_display) {
      std::string prop = postData[cobra::drampconstants::params][0][cobra::drampconstants::name];
      if (prop == cobra::drampconstants::param_pattern_type) {
          std::string val = postData[cobra::drampconstants::params][0][cobra::drampconstants::value];
          return CreatePOSTCommandResponse("/data/display/pattern/type/value", cmd, val, msgid, sendInProgressOnly);
      } else if (prop == cobra::drampconstants::param_pattern_rgb_red) {
          int val = postData[cobra::drampconstants::params][0][cobra::drampconstants::value].get<int>();
          return CreatePOSTCommandResponse("/data/display/pattern/rgbRed/value", cmd, val, msgid, sendInProgressOnly);
      } else if (prop == cobra::drampconstants::param_pattern_rgb_green) {
          int val = postData[cobra::drampconstants::params][0][cobra::drampconstants::value].get<int>();
          return CreatePOSTCommandResponse("/data/display/pattern/rgbGreen/value", cmd, val, msgid, sendInProgressOnly);
      } else if (prop == cobra::drampconstants::param_pattern_rgb_blue) {
          int val = postData[cobra::drampconstants::params][0][cobra::drampconstants::value].get<int>();
          return CreatePOSTCommandResponse("/data/display/pattern/rgbBlue/value", cmd, val, msgid, sendInProgressOnly);
      } else if (prop == cobra::drampconstants::param_orientation) {
          std::string val = postData[cobra::drampconstants::params][0][cobra::drampconstants::value];
          return CreatePOSTCommandResponse("/data/display/orientation/value", cmd, val, msgid, sendInProgressOnly);
      } else if (prop == cobra::drampconstants::res_path_display_pattern) {
          // nlohmann::json val = postData[cobra::drampconstants::params][0][cobra::drampconstants::value];
          return CreatePOSTCommandResponse("/data/display/pattern/value", cmd, 0, msgid, sendInProgressOnly);
      } else if (prop == cobra::drampconstants::param_scan_mode) {
          std::string val = postData[cobra::drampconstants::params][0][cobra::drampconstants::value];
          return CreatePOSTCommandResponse("/data/display/scanmode/value", cmd, val, msgid, sendInProgressOnly);
      } else if (prop == cobra::drampconstants::param_scan_mode_enabled) {
          bool val = postData[cobra::drampconstants::params][0][cobra::drampconstants::value].get<bool>();
          return CreatePOSTCommandResponse("/data/display/scanmodeenabled/value", cmd, val, msgid,
                                            sendInProgressOnly);
      } else if (prop == cobra::drampconstants::param_bezel_comp_pixels) {
          // int val = postData[cobra::drampconstants::params][0][cobra::drampconstants::value].get<int>();
          return CreatePOSTCommandResponse("/data/display/bezelComp/pixels/value", cmd, 0, msgid,
                                            sendInProgressOnly);
      } else if (prop == cobra::drampconstants::param_osi_text) {
          return CreatePOSTCommandResponse("/data/display/ositext/value", cmd, 0, msgid,
                                            sendInProgressOnly);
      } else if (prop == cobra::drampconstants::param_osi_nosignal) {
          return CreatePOSTCommandResponse("/data/display/nosignalosienabled/value", cmd, 0, msgid,
                                            sendInProgressOnly);
      } else if (prop == cobra::drampconstants::param_osi_timinginfo) {
        return CreatePOSTCommandResponse("/data/display/timinginfoosienabled/value", cmd, 0, msgid,
                                          sendInProgressOnly);
      } else if (prop == cobra::drampconstants::param_screentype) {
          return CreatePOSTCommandResponse("/data/display/screentype/value", cmd, 0, msgid,
                                            sendInProgressOnly);
      } else if (prop == cobra::drampconstants::param_uniformityCorrStrength) {
          return CreatePOSTCommandResponse("/data/display/uniformitycorrstrength/value", cmd, 0, msgid,
                                            sendInProgressOnly);
      }
  }  else if (cmd == cobra::drampconstants::update_image) {
      std::string input;
      for (auto& el : postData[cobra::drampconstants::params].items()) {
        if (el.value()["name"] == "pInput") {
          input = el.value()["value"].get<std::string>();
          std::transform(input.begin(), input.end(), input.begin(), ::toupper);
      }

      for (auto& el : postData[cobra::drampconstants::params].items()) {
        if (el.value()["name"] == "pDisplayMode") {
          std::string val = el.value()["value"].get<std::string>();
          std::transform(val.begin(), val.end(), val.begin(), ::toupper);
          return CreatePOSTCommandResponse("/data/image/"+ input +
             "/displayMode/value", cmd, val, msgid, sendInProgressOnly);
        } else {
          return CreatePOSTCommandResponse("/data/image/"+ input +
             "/tiling/value", cmd, 0, msgid, sendInProgressOnly);
        }
      }
    }
  }  else if (cmd == cobra::drampconstants::update_edid) {
      std::string input;
      for (auto& el : postData[cobra::drampconstants::params].items()) {
        if (el.value()["name"] == "pInput") {
          input = el.value()["value"].get<std::string>();
          std::transform(input.begin(), input.end(), input.begin(), ::toupper);
      }

      for (auto& el : postData[cobra::drampconstants::params].items()) {
        if (el.value()["name"] == "pName") {
          std::string val = el.value()["value"].get<std::string>();
          std::transform(val.begin(), val.end(), val.begin(), ::toupper);
          return CreatePOSTCommandResponse("/data/input/"+ input +
             "/edid/name/value", cmd, val, msgid, sendInProgressOnly);
        }
      }
    }
  } else if (cmd == cobra::drampconstants::identify) {
      // std::string val = postData[cobra::drampconstants::params][0][cobra::drampconstants::value];
      return CreatePOSTCommandResponse("", cmd, 0, msgid, sendInProgressOnly);
  } else if (cmd == cobra::drampconstants::triggerAutoAdjust) {
      return CreatePOSTCommandResponse("", cmd, 0, msgid, sendInProgressOnly);
  } else if (cmd == cobra::drampconstants::update_device) {
      return CreatePOSTCommandResponse("", cmd, 0, msgid, sendInProgressOnly);
  } else if (cmd == cobra::drampconstants::update_colorBrightness) {
      return CreatePOSTCommandResponse("", cmd, 0, msgid, sendInProgressOnly);
  } else if (cmd == cobra::drampconstants::update_geometry) {
      return CreatePOSTCommandResponse("", cmd, 0, msgid, sendInProgressOnly);
  } else if (cmd == cobra::drampconstants::resetColorBrightnessAdjusts) {
      return CreatePOSTCommandResponse("", cmd, 0, msgid, sendInProgressOnly);
  } else if (cmd == cobra::drampconstants::trigger_initial_setup) {
      return CreatePOSTCommandResponse("", cmd, 0, msgid, sendInProgressOnly);
  } else if (cmd == cobra::drampconstants::update_output) {
    return CreatePOSTCommandResponse("", cmd, 0, msgid, sendInProgressOnly);
  } else if (cmd == cobra::drampconstants::trigger_sensex_init) {
      return CreatePOSTCommandResponse("", cmd, 0, msgid, sendInProgressOnly);
  }
  return "Invalid command";
}
void MockDisplayData::PrintDisplayData() {
  std::cout << displayData_.dump() << std::endl;
}

std::string MockDisplayData::CreateGETCommandResponse(std::string& req_url) {
  try {
    displayData_.clear();
    ReadMockDataBase();

    std::size_t pos = req_url.find("/data");
    std::string url = req_url.substr(pos);
    nlohmann::json::json_pointer ptr_path(url);
    nlohmann::json res = displayData_.at(ptr_path);

    std::string file_contents;
    if (req_url == "/dramp/2/data") {
      std::string str;
      std::ifstream inFile;
      inFile.open(std::string(TESTDATA) + std::string("/dummy_response_data.json"));
      if (!inFile) {
        std::cout << "Unable to open file";
        exit(1);  // terminate with error
      }

      while ( inFile >> str ) {
        file_contents += str;
      }
      inFile.close();
      return file_contents;
    }

    if (req_url == cobra::drampconstants::res_path_data_device) {
        return CreateResponseForDataDevice(res);
    } else if (req_url == cobra::drampconstants::res_path_input) {
        return CreateResponseForDataInput(res);
    } else if (req_url == cobra::drampconstants::res_path_display) {
        return CreateResponseForDisplay(res);
    } else if (req_url == "/dramp/2/data/component") {
        return CreateResponseForComponent(res);
    } else if (req_url == cobra::drampconstants::res_path_image) {
        return CreateResponseForImage(res);
    } else if ((req_url == cobra::drampconstants::res_path_device_neighbor_left) ||
              (req_url == cobra::drampconstants::res_path_device_neighbor_top) ||
              (req_url == cobra::drampconstants::res_path_device_neighbor_right) ||
              (req_url == cobra::drampconstants::res_path_device_neighbor_bottom)) {
        return CreateResponseForNeighbor(req_url, res);
    }
  } catch (std::exception &e) {
    COBRA_LOG_STR_ERROR("######exception : ");
  }
  return "";
}

std::string MockDisplayData::CreatePOSTCommandResponse(const std::string& path,
                                                       const std::string& cmd,
                                                       const POST_REQUEST_PARAM_VALUE & value,
                                                       const std::string& msgid,
                                                       bool sendInProgressOnly) {
  try {
  nlohmann::json::json_pointer ptr_path(path);
  if (std::holds_alternative<std::string>(value)) {
    displayData_.at(ptr_path) = std::get<std::string>(value);
  } else if (std::holds_alternative<int>(value)) {
      displayData_.at(ptr_path) = std::get<int>(value);
  } else if (std::holds_alternative<bool>(value)) {
      displayData_.at(ptr_path) = std::get<bool>(value);
  } else if (std::holds_alternative<double>(value)) {
      displayData_.at(ptr_path) = std::get<double>(value);
  } else if (std::holds_alternative<float>(value)) {
      displayData_.at(ptr_path) = std::get<float>(value);
  } else {
     // TODO(ABSR) : do nothing currently
  }
  } catch(std::exception &e) {
      // path is empty so no data will be set in local database
  }
  // PrintDisplayData();
  std::string response;
  if (sendInProgressOnly) {
    response = "{"
      "\"resource\" : {"
      " \"name\" : \"ws://localhost:8080/dramp/2/actions/" + msgid + "\""
      "},"
      "\"action\" : {"
      " \"name\" : \"" + cmd + "\","
      " \"state\" : \"STATE_IN_PROGRESS\","
      " \"seq\" : 0 ,"
      " \"value\" : 0" +
      "}"
    "}";
  } else {
      response = "{"
        "\"resource\" : {"
        " \"name\" : \"ws://localhost:8080/dramp/2/actions/" + msgid + "\""
        "},"
        "\"action\" : {"
        " \"name\" : \"" + cmd + "\","
        " \"state\" : \"STATE_REQUEST_DONE\","
        " \"seq\" : 0 ,"
        " \"value\" : 0 " +
        "}"
      "}";
  }
  return response;
}

std::string MockDisplayData::CreateResponseForDataDevice(nlohmann::json & display_data) {
  nlohmann::json response_json = nlohmann::json::object();
  response_json["resource"]["name"] = "ws://localhost:8080" + cobra::drampconstants::res_path_data_device;

  nlohmann::json softwareVersion = nlohmann::json::object();
  softwareVersion["name"] = "/dramp/2/data/device/version/softwareVersion";
  softwareVersion["state"] = display_data["version"]["softwareVersion"]["state"];
  softwareVersion["value"] = display_data["version"]["softwareVersion"]["value"];
  softwareVersion["seq"] = "0";
  response_json["params"].push_back(softwareVersion);

  nlohmann::json softwareVersionsoc = nlohmann::json::object();
  softwareVersionsoc["name"] = "/dramp/2/data/device/version/softwareVersionsoc";
  softwareVersionsoc["state"] = display_data["version"]["softwareVersionsoc"]["state"];
  softwareVersionsoc["value"] = display_data["version"]["softwareVersionsoc"]["value"];
  softwareVersionsoc["seq"] = "0";
  response_json["params"].push_back(softwareVersionsoc);

  nlohmann::json softwareVersioncpu = nlohmann::json::object();
  softwareVersioncpu["name"] = "/dramp/2/data/device/version/cpusoftwareVersion";
  softwareVersioncpu["state"] = display_data["version"]["cpusoftwareVersion"]["state"];
  softwareVersioncpu["value"] = display_data["version"]["cpusoftwareVersion"]["value"];
  softwareVersioncpu["seq"] = "0";
  response_json["params"].push_back(softwareVersioncpu);

  nlohmann::json apiVersion = nlohmann::json::object();
  apiVersion["name"] = "/dramp/2/data/device/version/apiVersion";
  apiVersion["state"] = display_data["version"]["apiVersion"]["state"];
  apiVersion["value"] = display_data["version"]["apiVersion"]["value"];
  apiVersion["seq"] = "0";
  response_json["params"].push_back(apiVersion);

  nlohmann::json fpgaVersion = nlohmann::json::object();
  fpgaVersion["name"] = "/dramp/2/data/device/version/fpgaVersion";
  fpgaVersion["state"] = display_data["version"]["fpgaVersion"]["state"];
  fpgaVersion["value"] = display_data["version"]["fpgaVersion"]["value"];
  fpgaVersion["seq"] = "0";
  response_json["params"].push_back(fpgaVersion);
/*
  nlohmann::json fpgaMuxVersion_val = nlohmann::json::parse(
    "{"
      "\"name\" : \"ws://localhost:8080\","
      "\"state\" : \"STATE_NOT_AVAILABLE\","
      "\"seq\" : 0"
    "}");
*/
  nlohmann::json fpgaMuxVersion = nlohmann::json::object();
  fpgaMuxVersion["name"] = "/dramp/2/data/device/version/fpgaMuxVersion";
  fpgaMuxVersion["state"] = display_data["version"]["fpgaMuxVersion"]["state"];
  fpgaMuxVersion["seq"] = "0";
  response_json["params"].push_back(fpgaMuxVersion);

  nlohmann::json kernelVersion = nlohmann::json::object();
  kernelVersion["name"] = "/dramp/2/data/device/version/kernelVersion";
  kernelVersion["state"] = display_data["version"]["kernelVersion"]["state"];
  kernelVersion["value"] = display_data["version"]["kernelVersion"]["value"];
  kernelVersion["seq"] = "0";
  response_json["params"].push_back(kernelVersion);

  nlohmann::json ubootVersion = nlohmann::json::object();
  ubootVersion["name"] = "/dramp/2/data/device/version/ubootVersion";
  ubootVersion["state"] = display_data["version"]["ubootVersion"]["state"];
  ubootVersion["value"] = display_data["version"]["ubootVersion"]["value"];
  ubootVersion["seq"] = "0";
  response_json["params"].push_back(ubootVersion);

  nlohmann::json ubootEnvVersion = nlohmann::json::object();
  ubootEnvVersion["name"] = "/dramp/2/data/device/version/ubootEnvVersion";
  ubootEnvVersion["state"] = display_data["version"]["ubootEnvVersion"]["state"];
  ubootEnvVersion["value"] = display_data["version"]["ubootEnvVersion"]["value"];
  ubootEnvVersion["seq"] = "0";
  response_json["params"].push_back(ubootEnvVersion);

  nlohmann::json devicetype = nlohmann::json::object();
  devicetype["name"] = "/dramp/2/data/device/devicetype";
  devicetype["state"] = display_data["devicetype"]["state"];
  devicetype["value"] = display_data["devicetype"]["value"];
  devicetype["seq"] = "0";
  response_json["params"].push_back(devicetype);

  nlohmann::json deviceTypeVariant = nlohmann::json::object();
  deviceTypeVariant["name"] = "/dramp/2/data/device/deviceTypeVariant";
  deviceTypeVariant["state"] = display_data["deviceTypeVariant"]["state"];
  deviceTypeVariant["value"] = display_data["deviceTypeVariant"]["value"];
  deviceTypeVariant["seq"] = "0";
  response_json["params"].push_back(deviceTypeVariant);

  nlohmann::json serial = nlohmann::json::object();
  serial["name"] = "/dramp/2/data/device/serial";
  serial["state"] = display_data["serial"]["state"];
  serial["value"] = display_data["serial"]["value"];
  serial["seq"] = "0";
  response_json["params"].push_back(serial);

  nlohmann::json uid = nlohmann::json::object();
  uid["name"] = "/dramp/2/data/device/uid";
  uid["state"] = display_data["uid"]["state"];
  uid["value"] = display_data["uid"]["value"];
  uid["seq"] = "0";
  response_json["params"].push_back(uid);

  nlohmann::json operationState = nlohmann::json::object();
  operationState["name"] = "/dramp/2/data/device/operationState";
  operationState["state"] = display_data["operationstate"]["state"];
  operationState["value"] = display_data["operationstate"]["value"];
  operationState["seq"] = "0";
  response_json["params"].push_back(operationState);

  nlohmann::json startupState = nlohmann::json::object();
  startupState["name"] = "/dramp/2/data/device/startupState";
  startupState["state"] = display_data["startupState"]["state"];
  startupState["value"] = display_data["startupState"]["value"];
  startupState["seq"] = "0";
  response_json["params"].push_back(startupState);

  nlohmann::json powerMode = nlohmann::json::object();
  powerMode["name"] = "/dramp/2/data/device/powerMode";
  powerMode["state"] = display_data["powerMode"]["state"];
  powerMode["value"] = display_data["powerMode"]["value"];
  powerMode["seq"] = "0";
  response_json["params"].push_back(powerMode);

  nlohmann::json overtemperatureProtection = nlohmann::json::object();
  overtemperatureProtection["name"] = "/dramp/2/data/device/overtemperatureProtection";
  overtemperatureProtection["state"] = display_data["overtemperatureProtection"]["state"];
  overtemperatureProtection["value"] = display_data["overtemperatureProtection"]["value"];
  overtemperatureProtection["seq"] = "0";
  response_json["params"].push_back(overtemperatureProtection);

  nlohmann::json capability = nlohmann::json::object();
  capability["name"] = cobra::drampconstants::res_path_display_capabilities;
  capability["state"] = display_data["capabilities"]["state"];
  capability["value"] = display_data["capabilities"]["value"];
  capability["seq"] = "0";
  response_json["params"].push_back(capability);

  nlohmann::json leftNeighbor = nlohmann::json::object();
  leftNeighbor["name"] = "/dramp/2/data/device/neighbor/left";
  leftNeighbor["state"] = display_data["neighbor"]["left"]["state"];
  leftNeighbor["value"] = display_data["neighbor"]["left"]["value"];
  leftNeighbor["seq"] = "0";
  response_json["params"].push_back(leftNeighbor);

  nlohmann::json topNeighbor = nlohmann::json::object();
  topNeighbor["name"] = "/dramp/2/data/device/neighbor/top";
  topNeighbor["state"] = display_data["neighbor"]["top"]["state"];
  topNeighbor["value"] = display_data["neighbor"]["top"]["value"];
  topNeighbor["seq"] = "0";
  response_json["params"].push_back(topNeighbor);

  nlohmann::json rightNeighbor = nlohmann::json::object();
  rightNeighbor["name"] = "/dramp/2/data/device/neighbor/right";
  rightNeighbor["state"] = display_data["neighbor"]["right"]["state"];
  rightNeighbor["value"] = display_data["neighbor"]["right"]["value"];
  rightNeighbor["seq"] = "0";
  response_json["params"].push_back(rightNeighbor);

  nlohmann::json bottomNeighbor = nlohmann::json::object();
  bottomNeighbor["name"] = "/dramp/2/data/device/neighbor/bottom";
  bottomNeighbor["state"] = display_data["neighbor"]["bottom"]["state"];
  bottomNeighbor["value"] = display_data["neighbor"]["bottom"]["value"];
  bottomNeighbor["seq"] = "0";
  response_json["params"].push_back(bottomNeighbor);

  return response_json.dump();
}

std::string MockDisplayData::CreateResponseForDisplay(nlohmann::json & display_data) {
  nlohmann::json response_json = nlohmann::json::object();
  response_json["resource"]["name"] = "ws://localhost:8080" + cobra::drampconstants::res_path_display;

  nlohmann::json json_pattern_type = nlohmann::json::object();
  json_pattern_type["name"] = "/dramp/2/data/display/pattern/type";
  json_pattern_type["state"] = display_data["pattern"]["type"]["state"];
  json_pattern_type["value"] = display_data["pattern"]["type"]["value"];
  json_pattern_type["seq"] = "0";
  response_json["params"].push_back(json_pattern_type);

  nlohmann::json json_pattern_rgbRed = nlohmann::json::object();
  json_pattern_rgbRed["name"] = "/dramp/2/data/display/pattern/rgbRed";
  json_pattern_rgbRed["state"] = display_data["pattern"]["rgbRed"]["state"];
  json_pattern_rgbRed["value"] = display_data["pattern"]["rgbRed"]["value"];
  json_pattern_rgbRed["seq"] = "0";
  response_json["params"].push_back(json_pattern_rgbRed);

  nlohmann::json json_pattern_rgbGreen = nlohmann::json::object();
  json_pattern_rgbGreen["name"] = "/dramp/2/data/display/pattern/rgbGreen";
  json_pattern_rgbGreen["state"] = display_data["pattern"]["rgbGreen"]["state"];
  json_pattern_rgbGreen["value"] = display_data["pattern"]["rgbGreen"]["value"];
  json_pattern_rgbGreen["seq"] = "0";
  response_json["params"].push_back(json_pattern_rgbGreen);

  nlohmann::json json_pattern_rgbBlue = nlohmann::json::object();
  json_pattern_rgbBlue["name"] = "/dramp/2/data/display/pattern/rgbBlue";
  json_pattern_rgbBlue["state"] = display_data["pattern"]["rgbBlue"]["state"];
  json_pattern_rgbBlue["value"] = display_data["pattern"]["rgbBlue"]["value"];
  json_pattern_rgbBlue["seq"] = "0";
  response_json["params"].push_back(json_pattern_rgbBlue);

  nlohmann::json json_orientation = nlohmann::json::object();
  json_orientation["name"] = "/dramp/2/data/display/orientation";
  json_orientation["state"] = display_data["orientation"]["state"];
  json_orientation["value"] = display_data["orientation"]["value"];
  json_orientation["seq"] = "0";
  response_json["params"].push_back(json_orientation);

  nlohmann::json json_scanmode = nlohmann::json::object();
  json_scanmode["name"] = "/dramp/2/data/display/scanmode";
  json_scanmode["state"] = display_data["scanmode"]["state"];
  json_scanmode["value"] = display_data["scanmode"]["value"];
  json_scanmode["seq"] = "0";
  response_json["params"].push_back(json_scanmode);

  nlohmann::json json_scanmodeenabled = nlohmann::json::object();
  json_scanmode["name"] = "/dramp/2/data/display/scanmodeenabled";
  json_scanmode["state"] = display_data["scanmodeenabled"]["state"];
  json_scanmode["value"] = display_data["scanmodeenabled"]["value"];
  json_scanmode["seq"] = "0";
  response_json["params"].push_back(json_scanmode);

  nlohmann::json bezelPixels = nlohmann::json::object();
  bezelPixels["name"] = "/dramp/2/data/display/bezelComp/pixels";
  bezelPixels["state"] = display_data["bezelComp"]["pixels"]["state"];
  bezelPixels["value"] = display_data["bezelComp"]["pixels"]["value"];
  bezelPixels["seq"] = "0";
  response_json["params"].push_back(bezelPixels);

  nlohmann::json bezelSizeWidth = nlohmann::json::object();
  bezelSizeWidth["name"] = "/dramp/2/data/display/bezelComp/sizeWidth";
  bezelSizeWidth["state"] = display_data["bezelComp"]["sizeWidth"]["state"];
  bezelSizeWidth["value"] = display_data["bezelComp"]["sizeWidth"]["value"];
  bezelSizeWidth["seq"] = "0";
  response_json["params"].push_back(bezelSizeWidth);

  nlohmann::json bezelSizeHeight = nlohmann::json::object();
  bezelSizeHeight["name"] = "/dramp/2/data/display/bezelComp/sizeHeight";
  bezelSizeHeight["state"] = display_data["bezelComp"]["sizeHeight"]["state"];
  bezelSizeHeight["value"] = display_data["bezelComp"]["sizeHeight"]["value"];
  bezelSizeHeight["seq"] = "0";
  response_json["params"].push_back(bezelSizeHeight);

  nlohmann::json bezelPositionX = nlohmann::json::object();
  bezelPositionX["name"] = "/dramp/2/data/display/bezelComp/positionX";
  bezelPositionX["state"] = display_data["bezelComp"]["positionX"]["state"];
  bezelPositionX["value"] = display_data["bezelComp"]["positionX"]["value"];
  bezelPositionX["seq"] = "0";
  response_json["params"].push_back(bezelPositionX);

  nlohmann::json bezelPositionY = nlohmann::json::object();
  bezelPositionY["name"] = "/dramp/2/data/display/bezelComp/positionY";
  bezelPositionY["state"] = display_data["bezelComp"]["positionY"]["state"];
  bezelPositionY["value"] = display_data["bezelComp"]["positionY"]["value"];
  bezelPositionY["seq"] = "0";
  response_json["params"].push_back(bezelPositionY);

  nlohmann::json json_activeorientation = nlohmann::json::object();
  json_activeorientation["name"] = cobra::drampconstants::res_path_display_active_orientation;
  json_activeorientation["state"] = display_data["activeorientation"]["state"];
  json_activeorientation["value"] = display_data["activeorientation"]["value"];
  json_activeorientation["seq"] = "0";
  response_json["params"].push_back(json_activeorientation);

  return response_json.dump();
}

std::string MockDisplayData::CreateResponseForDataInput(nlohmann::json & display_data) {
  nlohmann::json response_json = nlohmann::json::object();
  response_json["resource"]["name"] = "ws://localhost:8080" + cobra::drampconstants::res_path_input;

  nlohmann::json selectedInput = nlohmann::json::object();
  selectedInput["name"] = "/dramp/2/data/input/selectedInput";
  selectedInput["state"] = display_data["selectedInput"]["state"];
  selectedInput["value"] = display_data["selectedInput"]["value"];
  selectedInput["seq"] = "0";
  response_json["params"].push_back(selectedInput);

  nlohmann::json activeInput = nlohmann::json::object();
  activeInput["name"] = "/dramp/2/data/input/activeInput";
  activeInput["state"] = display_data["activeInput"]["state"];
  activeInput["value"] = display_data["activeInput"]["value"];
  activeInput["seq"] = "0";
  response_json["params"].push_back(activeInput);

  nlohmann::json inputMode = nlohmann::json::object();
  inputMode["name"] = "/dramp/2/data/input/inputMode";
  inputMode["state"] = display_data["inputMode"]["state"];
  inputMode["value"] = display_data["inputMode"]["value"];
  inputMode["seq"] = "0";
  response_json["params"].push_back(inputMode);
  return response_json.dump();
}

std::string  MockDisplayData::CreateResponseForComponent(nlohmann::json& display_data) {
  nlohmann::json response_json = nlohmann::json::object();
  response_json["resource"]["name"] = "ws://localhost:8080" + std::string("/dramp/2/data/component");

  nlohmann::json serialnumber = nlohmann::json::object();
  serialnumber["name"] = "/dramp/2/data/component/serialnumber";
  serialnumber["state"] = display_data["serialnumber"]["state"];
  serialnumber["value"] = display_data["serialnumber"]["value"];
  serialnumber["seq"] = "0";
  response_json["params"].push_back(serialnumber);

  nlohmann::json articlenumber = nlohmann::json::object();
  articlenumber["name"] = "/dramp/2/data/component/articlenumber";
  articlenumber["state"] = display_data["articlenumber"]["state"];
  articlenumber["value"] = display_data["articlenumber"]["value"];
  articlenumber["seq"] = "0";
  response_json["params"].push_back(articlenumber);
  return response_json.dump();
}

std::string  MockDisplayData::CreateResponseForImage(nlohmann::json& display_data) {
  nlohmann::json response_json = nlohmann::json::object();
  response_json["resource"]["name"] = "ws://localhost:8080" + std::string("/dramp/2/data/image");

  nlohmann::json displayport1_displaymode = nlohmann::json::object();
  displayport1_displaymode["name"] = "/dramp/2/data/image/DISPLAYPORT1/displayMode";
  displayport1_displaymode["state"] = display_data["DISPLAYPORT1"]["displayMode"]["state"];
  displayport1_displaymode["value"] = display_data["DISPLAYPORT1"]["displayMode"]["value"];
  displayport1_displaymode["seq"] = "0";
  response_json["params"].push_back(displayport1_displaymode);

  nlohmann::json displayport2_displaymode = nlohmann::json::object();
  displayport2_displaymode["name"] = "/dramp/2/data/image/DISPLAYPORT2/displayMode";
  displayport2_displaymode["state"] = display_data["DISPLAYPORT2"]["displayMode"]["state"];
  displayport2_displaymode["value"] = display_data["DISPLAYPORT2"]["displayMode"]["value"];
  displayport2_displaymode["seq"] = "0";
  response_json["params"].push_back(displayport2_displaymode);

  nlohmann::json hdmi1_displaymode = nlohmann::json::object();
  hdmi1_displaymode["name"] = "/dramp/2/data/image/HDMI1/displayMode";
  hdmi1_displaymode["state"] = display_data["HDMI1"]["displayMode"]["state"];
  hdmi1_displaymode["value"] = display_data["HDMI1"]["displayMode"]["value"];
  hdmi1_displaymode["seq"] = "0";
  response_json["params"].push_back(hdmi1_displaymode);

  nlohmann::json hdmi2_displaymode = nlohmann::json::object();
  hdmi2_displaymode["name"] = "/dramp/2/data/image/HDMI2/displayMode";
  hdmi2_displaymode["state"] = display_data["HDMI2"]["displayMode"]["state"];
  hdmi2_displaymode["value"] = display_data["HDMI2"]["displayMode"]["value"];
  hdmi2_displaymode["seq"] = "0";
  response_json["params"].push_back(hdmi2_displaymode);

  nlohmann::json displayport1_tiling_sizeWidth = nlohmann::json::object();
  displayport1_tiling_sizeWidth["name"] = "/dramp/2/data/image/DISPLAYPORT1/tiling/sizeWidth";
  displayport1_tiling_sizeWidth["state"] = display_data["DISPLAYPORT1"]["tiling"]["sizeWidth"]["state"];
  displayport1_tiling_sizeWidth["value"] = display_data["DISPLAYPORT1"]["tiling"]["sizeWidth"]["value"];
  displayport1_tiling_sizeWidth["seq"] = "0";
  response_json["params"].push_back(displayport1_tiling_sizeWidth);

  nlohmann::json displayport1_tiling_sizeHeight = nlohmann::json::object();
  displayport1_tiling_sizeHeight["name"] = "/dramp/2/data/image/DISPLAYPORT1/tiling/sizeHeight";
  displayport1_tiling_sizeHeight["state"] = display_data["DISPLAYPORT1"]["tiling"]["sizeHeight"]["state"];
  displayport1_tiling_sizeHeight["value"] = display_data["DISPLAYPORT1"]["tiling"]["sizeHeight"]["value"];
  displayport1_tiling_sizeHeight["seq"] = "0";
  response_json["params"].push_back(displayport1_tiling_sizeHeight);

  nlohmann::json displayport1_tiling_positionX = nlohmann::json::object();
  displayport1_tiling_positionX["name"] = "/dramp/2/data/image/DISPLAYPORT1/tiling/positionX";
  displayport1_tiling_positionX["state"] = display_data["DISPLAYPORT1"]["tiling"]["positionX"]["state"];
  displayport1_tiling_positionX["value"] = display_data["DISPLAYPORT1"]["tiling"]["positionX"]["value"];
  displayport1_tiling_positionX["seq"] = "0";
  response_json["params"].push_back(displayport1_tiling_positionX);

  nlohmann::json displayport1_tiling_positionY = nlohmann::json::object();
  displayport1_tiling_positionY["name"] = "/dramp/2/data/image/DISPLAYPORT1/tiling/positionY";
  displayport1_tiling_positionY["state"] = display_data["DISPLAYPORT1"]["tiling"]["positionY"]["state"];
  displayport1_tiling_positionY["value"] = display_data["DISPLAYPORT1"]["tiling"]["positionY"]["value"];
  displayport1_tiling_positionY["seq"] = "0";
  response_json["params"].push_back(displayport1_tiling_positionY);

  nlohmann::json displayport2_tiling_sizeWidth = nlohmann::json::object();
  displayport2_tiling_sizeWidth["name"] = "/dramp/2/data/image/DISPLAYPORT2/tiling/sizeWidth";
  displayport2_tiling_sizeWidth["state"] = display_data["DISPLAYPORT2"]["tiling"]["sizeWidth"]["state"];
  displayport2_tiling_sizeWidth["value"] = display_data["DISPLAYPORT2"]["tiling"]["sizeWidth"]["value"];
  displayport2_tiling_sizeWidth["seq"] = "0";
  response_json["params"].push_back(displayport2_tiling_sizeWidth);

  nlohmann::json displayport2_tiling_sizeHeight = nlohmann::json::object();
  displayport2_tiling_sizeHeight["name"] = "/dramp/2/data/image/DISPLAYPORT2/tiling/sizeHeight";
  displayport2_tiling_sizeHeight["state"] = display_data["DISPLAYPORT2"]["tiling"]["sizeHeight"]["state"];
  displayport2_tiling_sizeHeight["value"] = display_data["DISPLAYPORT2"]["tiling"]["sizeHeight"]["value"];
  displayport2_tiling_sizeHeight["seq"] = "0";
  response_json["params"].push_back(displayport2_tiling_sizeHeight);

  nlohmann::json displayport2_tiling_positionX = nlohmann::json::object();
  displayport2_tiling_positionX["name"] = "/dramp/2/data/image/DISPLAYPORT2/tiling/positionX";
  displayport2_tiling_positionX["state"] = display_data["DISPLAYPORT2"]["tiling"]["positionX"]["state"];
  displayport2_tiling_positionX["value"] = display_data["DISPLAYPORT2"]["tiling"]["positionX"]["value"];
  displayport2_tiling_positionX["seq"] = "0";
  response_json["params"].push_back(displayport2_tiling_positionX);

  nlohmann::json displayport2_tiling_positionY = nlohmann::json::object();
  displayport2_tiling_positionY["name"] = "/dramp/2/data/image/DISPLAYPORT2/tiling/positionY";
  displayport2_tiling_positionY["state"] = display_data["DISPLAYPORT2"]["tiling"]["positionY"]["state"];
  displayport2_tiling_positionY["value"] = display_data["DISPLAYPORT2"]["tiling"]["positionY"]["value"];
  displayport2_tiling_positionY["seq"] = "0";
  response_json["params"].push_back(displayport2_tiling_positionY);

  nlohmann::json hdmi1_tiling_sizeWidth = nlohmann::json::object();
  hdmi1_tiling_sizeWidth["name"] = "/dramp/2/data/image/HDMI1/tiling/sizeWidth";
  hdmi1_tiling_sizeWidth["state"] = display_data["HDMI1"]["tiling"]["sizeWidth"]["state"];
  hdmi1_tiling_sizeWidth["value"] = display_data["HDMI1"]["tiling"]["sizeWidth"]["value"];
  hdmi1_tiling_sizeWidth["seq"] = "0";
  response_json["params"].push_back(hdmi1_tiling_sizeWidth);

  nlohmann::json hdmi1_tiling_sizeHeight = nlohmann::json::object();
  hdmi1_tiling_sizeHeight["name"] = "/dramp/2/data/image/HDMI1/tiling/sizeHeight";
  hdmi1_tiling_sizeHeight["state"] = display_data["HDMI1"]["tiling"]["sizeHeight"]["state"];
  hdmi1_tiling_sizeHeight["value"] = display_data["HDMI1"]["tiling"]["sizeHeight"]["value"];
  hdmi1_tiling_sizeHeight["seq"] = "0";
  response_json["params"].push_back(hdmi1_tiling_sizeHeight);

  nlohmann::json hdmi1_tiling_positionX = nlohmann::json::object();
  hdmi1_tiling_positionX["name"] = "/dramp/2/data/image/HDMI1/tiling/positionX";
  hdmi1_tiling_positionX["state"] = display_data["HDMI1"]["tiling"]["positionX"]["state"];
  hdmi1_tiling_positionX["value"] = display_data["HDMI1"]["tiling"]["positionX"]["value"];
  hdmi1_tiling_positionX["seq"] = "0";
  response_json["params"].push_back(hdmi1_tiling_positionX);

  nlohmann::json hdmi1_tiling_positionY = nlohmann::json::object();
  hdmi1_tiling_positionY["name"] = "/dramp/2/data/image/HDMI1/tiling/positionY";
  hdmi1_tiling_positionY["state"] = display_data["HDMI1"]["tiling"]["positionY"]["state"];
  hdmi1_tiling_positionY["value"] = display_data["HDMI1"]["tiling"]["positionY"]["value"];
  hdmi1_tiling_positionY["seq"] = "0";
  response_json["params"].push_back(hdmi1_tiling_positionY);

  nlohmann::json hdmi2_tiling_sizeWidth = nlohmann::json::object();
  hdmi2_tiling_sizeWidth["name"] = "/dramp/2/data/image/HDMI2/tiling/sizeWidth";
  hdmi2_tiling_sizeWidth["state"] = display_data["HDMI2"]["tiling"]["sizeWidth"]["state"];
  hdmi2_tiling_sizeWidth["value"] = display_data["HDMI2"]["tiling"]["sizeWidth"]["value"];
  hdmi2_tiling_sizeWidth["seq"] = "0";
  response_json["params"].push_back(hdmi2_tiling_sizeWidth);

  nlohmann::json hdmi2_tiling_sizeHeight = nlohmann::json::object();
  hdmi2_tiling_sizeHeight["name"] = "/dramp/2/data/image/HDMI2/tiling/sizeHeight";
  hdmi2_tiling_sizeHeight["state"] = display_data["HDMI2"]["tiling"]["sizeHeight"]["state"];
  hdmi2_tiling_sizeHeight["value"] = display_data["HDMI2"]["tiling"]["sizeHeight"]["value"];
  hdmi2_tiling_sizeHeight["seq"] = "0";
  response_json["params"].push_back(hdmi2_tiling_sizeHeight);

  nlohmann::json hdmi2_tiling_positionX = nlohmann::json::object();
  hdmi2_tiling_positionX["name"] = "/dramp/2/data/image/HDMI2/tiling/positionX";
  hdmi2_tiling_positionX["state"] = display_data["HDMI2"]["tiling"]["positionX"]["state"];
  hdmi2_tiling_positionX["value"] = display_data["HDMI2"]["tiling"]["positionX"]["value"];
  hdmi2_tiling_positionX["seq"] = "0";
  response_json["params"].push_back(hdmi2_tiling_positionX);

  nlohmann::json hdmi2_tiling_positionY = nlohmann::json::object();
  hdmi2_tiling_positionY["name"] = "/dramp/2/data/image/HDMI2/tiling/positionY";
  hdmi2_tiling_positionY["state"] = display_data["HDMI2"]["tiling"]["positionY"]["state"];
  hdmi2_tiling_positionY["value"] = display_data["HDMI2"]["tiling"]["positionY"]["value"];
  hdmi2_tiling_positionY["seq"] = "0";
  response_json["params"].push_back(hdmi2_tiling_positionY);

  return response_json.dump();
}

std::string MockDisplayData::CreateResponseForNeighbor(const std::string& res_url, nlohmann::json & display_data) {
  nlohmann::json response_json = nlohmann::json::object();

  if (res_url == cobra::drampconstants::res_path_device_neighbor_left) {
    response_json["resource"]["name"] = "ws://localhost:8080" + res_url;
    nlohmann::json neighbor = nlohmann::json::object();
    neighbor["name"] = cobra::drampconstants::res_path_device_neighbor_left;
    neighbor["state"] = display_data["state"];
    neighbor["value"] = display_data["value"];
    neighbor["seq"] = "0";
    response_json["params"].push_back(neighbor);
  } else if (res_url == cobra::drampconstants::res_path_device_neighbor_top) {
      response_json["resource"]["name"] = "ws://localhost:8080" + res_url;
      nlohmann::json neighbor = nlohmann::json::object();
      neighbor["name"] = cobra::drampconstants::res_path_device_neighbor_top;
      neighbor["state"] = display_data["state"];
      neighbor["value"] = display_data["value"];
      neighbor["seq"] = "0";
      response_json["params"].push_back(neighbor);
  } else if (res_url == cobra::drampconstants::res_path_device_neighbor_right) {
      response_json["resource"]["name"] = "ws://localhost:8080" + res_url;
      nlohmann::json neighbor = nlohmann::json::object();
      neighbor["name"] = cobra::drampconstants::res_path_device_neighbor_right;
      neighbor["state"] = display_data["state"];
      neighbor["value"] = display_data["value"];
      neighbor["seq"] = "0";
      response_json["params"].push_back(neighbor);
  } else if (res_url == cobra::drampconstants::res_path_device_neighbor_bottom) {
      response_json["resource"]["name"] = "ws://localhost:8080" + res_url;
      nlohmann::json neighbor = nlohmann::json::object();
      neighbor["name"] = cobra::drampconstants::res_path_device_neighbor_bottom;
      neighbor["state"] = display_data["state"];
      neighbor["value"] = display_data["value"];
      neighbor["seq"] = "0";
      response_json["params"].push_back(neighbor);
  }
  return response_json.dump();
}

void MockDisplayData::ReadMockDataBase() {
  std::string displayDataJsonFile =
    std::string(TESTDATA) + std::string("/dummy_display_database.json");
  std::ifstream ifs;  // Not configured to throw exceptions
  ifs.open(displayDataJsonFile, std::ios_base::in);
  COBRA_LOG_STR_TRACE("opening display data json file '" + displayDataJsonFile + "'");
  if (!ifs) {
    COBRA_LOG_STR_ERROR("Error while opening display data json file '" + displayDataJsonFile + "'");
    throw std::runtime_error("Error while opening display data json file '" + displayDataJsonFile + "'");
  } else {
    try {
      std::stringstream buffer;
      buffer << ifs.rdbuf();
      displayData_ = nlohmann::json::parse(buffer.str(), nullptr);
      ifs.close();
      // std::cout << "Successfully parsed display data json file '" + displayDataJsonFile + "'\n";
      COBRA_LOG_STR_TRACE("Successfully parsed display data json file : " + displayDataJsonFile);
    } catch(const nlohmann::json::parse_error& e1) {
      ifs.close();
      // std::cout << "Error while parsing display data json file '" + displayDataJsonFile + "' : " + e1.what();
      COBRA_LOG_STR_ERROR("Error while parsing display data json file : " + displayDataJsonFile +
        ". Exception : " + e1.what());
      throw std::runtime_error("Error while parsing display data json file '" + displayDataJsonFile + "' : "
        + e1.what());
    }
  }
}
