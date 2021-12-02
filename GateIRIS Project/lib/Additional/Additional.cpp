#include "Additional.hpp"

void add::Additional::viewTokenInfo(token_info_t token)
{
  switch (token.status)
  {
  case token_status_uninitialized:
    Serial.println("Token uninitialized.");
    break;

  case token_status_on_initialize:
    Serial.println("Token on initialize.");
    break;

  case token_status_on_signing:
    Serial.println("Token on signing.");
    break;

  case token_status_on_request:
    Serial.println("Token on request.");
    break;

  case token_status_on_refresh:
    Serial.println("Token on refresh.");
    break;

  case token_status_ready:
    Serial.println("Token ready.");
    break;

  case token_status_error:
    Serial.println("Token error.");
    break;

  default:
    Serial.println("Unidentified token status.");
    break;
  }

  switch (token.type)
  {
  case token_type_undefined:
    Serial.println("Token undefined");
    break;

  case token_type_legacy_token:
    Serial.println("Legacy token.");
    break;

  case token_type_id_token:
    Serial.println("ID Token.");
    break;

  case token_type_custom_token:
    Serial.println("Custom token.");
    break;

  case token_type_oauth2_access_token:
    Serial.println("OAuth2.0 access token.");
    break;

  case token_type_refresh_token:
    Serial.println("Refresh token.");
    break;

  default:
    Serial.println("Unrecognized token type.");
    break;
  }
}

String add::Additional::WiFiStatus()
{
  switch (WiFi.status())
  {
  case WL_IDLE_STATUS:
    return "Wifi inactive";
    break;

  case WL_NO_SSID_AVAIL:
    return "No SSID available";
    break;

  case WL_SCAN_COMPLETED:
    return "Scan completed";
    break;

  case WL_CONNECTED:
    return "Connected";
    break;

  case WL_CONNECT_FAILED:
    return "Connect failed";
    break;

  case WL_CONNECTION_LOST:
    return "Connection lost";
    break;

  case WL_DISCONNECTED:
    return "Disconnected";
    break;

  case WL_NO_SHIELD:
    return "No shield";
    break;

  default:
    return "Unknown error";
    break;
  }
}