#include "Additional.hpp"

void add::Additional::viewTokenInfo(token_info_t token)
{
  switch (token.status)
  {
  case token_status_uninitialized:
    Serial.println("Token nao inicializado.");
    break;

  case token_status_on_initialize:
    Serial.println("Token inicializando.");
    break;

  case token_status_on_signing:
    Serial.println("Token em assinatura.");
    break;

  case token_status_on_request:
    Serial.println("Token em solicitacao.");
    break;

  case token_status_on_refresh:
    Serial.println("Token atualizando.");
    break;

  case token_status_ready:
    Serial.println("Token pronto.");
    break;

  case token_status_error:
    Serial.println("Erro ao inicializar token.");
    break;

  default:
    Serial.println("Unidentified token status.");
    break;
  }

  switch (token.type)
  {
  case token_type_undefined:
    Serial.println("Token de tipo indefinido.");
    break;

  case token_type_legacy_token:
    Serial.println("Token de tipo legado.");
    break;

  case token_type_id_token:
    Serial.println("Token de tipo ID.");
    break;

  case token_type_custom_token:
    Serial.println("Token de tipo customizado.");
    break;

  case token_type_oauth2_access_token:
    Serial.println("Token de tipo OAuth2.0.");
    break;

  case token_type_refresh_token:
    Serial.println("Token de tipo atualizacao.");
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