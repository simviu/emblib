


#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>


#include "esp_wifi.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_netif.h"
//#include "protocol_examples_common.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"

#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"

#include "esp_log.h"
#include "mqtt_client.h"

#include "emb/esp32/embLib_esp.h"

using namespace emb;

namespace{
    //---- local instance
    class Inst{
    public:
        struct Data{
            bool isConnected = false;
        }; Data data_;
        MQTT::FunRecvCb f_recvCb_ = nullptr;
        esp_mqtt_client_handle_t client_;
    }; 
    Inst inst_;

}
//--------------
static void onConn_test()
{
    /*
    msg_id = esp_mqtt_client_publish(client, MQTT_T_TOPIC_PUB, "{spd:1.2}", 0, 1, 0);
    ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id);

    msg_id = esp_mqtt_client_subscribe(client, MQTT_T_TOPIC_SUB, 0);
    ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);
    */

//    msg_id = esp_mqtt_client_subscribe(client, "/topic/qos1", 1);
    //   ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);

//    msg_id = esp_mqtt_client_unsubscribe(client, "/topic/qos1");
//    ESP_LOGI(TAG, "sent unsubscribe successful, msg_id=%d", msg_id);
     
}

//--------------
static const char *TAG = "EMB_MQTT";
static void log_error_if_nonzero(const char *message, int error_code)
{
    if (error_code != 0) {
        ESP_LOGE(TAG, "Last error %s: 0x%x", message, error_code);
    }
}

//------
static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    ESP_LOGD(TAG, "Event dispatched from event loop base=%s, event_id=%" PRIi32 "", base, event_id);
    esp_mqtt_event_handle_t event = (esp_mqtt_event_handle_t)event_data;
    esp_mqtt_client_handle_t client = event->client;
    int msg_id;
    switch ((esp_mqtt_event_id_t)event_id) {
    case MQTT_EVENT_CONNECTED:
    //    onConn_test();
        inst_.data_.isConnected = true;
        ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
        break;
    case MQTT_EVENT_DISCONNECTED:
        inst_.data_.isConnected = false;
        ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
        break;

    case MQTT_EVENT_SUBSCRIBED:
        ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
        //msg_id = esp_mqtt_client_publish(client, "/topic/qos0", "data", 0, 0, 0);
        //ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id);
        break;
    case MQTT_EVENT_UNSUBSCRIBED:
        ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_PUBLISHED:
        ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_DATA:
        {
            ESP_LOGI(TAG, "MQTT_EVENT_DATA");
        //    printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
        //    printf("DATA=%.*s\r\n", event->data_len, event->data);
            if(inst_.f_recvCb_!=nullptr) 
                inst_.f_recvCb_(string(event->topic), 
                                string(event->data));
        }
        break;

    case MQTT_EVENT_ERROR:
        ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
        if (event->error_handle->error_type == MQTT_ERROR_TYPE_TCP_TRANSPORT) {
            log_error_if_nonzero("reported from esp-tls", event->error_handle->esp_tls_last_esp_err);
            log_error_if_nonzero("reported from tls stack", event->error_handle->esp_tls_stack_err);
            log_error_if_nonzero("captured as transport's socket errno",  event->error_handle->esp_transport_sock_errno);
            ESP_LOGI(TAG, "Last errno string (%s)", strerror(event->error_handle->esp_transport_sock_errno));

        }
        break;
    default:
        ESP_LOGI(TAG, "Other event id:%d", event->event_id);
        break;
    }
}
//--------
static void mqtt_app_start(const MQTT::Cfg& cfg)
{
    ESP_LOGI(TAG, "mqtt_app_start...");

    esp_mqtt_client_config_t mqtt_cfg;
    mqtt_cfg.broker.address.uri = cfg.sUrl.c_str(),
    mqtt_cfg.credentials.username = cfg.sUsr.c_str();
    mqtt_cfg.credentials.authentication.password = cfg.sPswd.c_str();
#if CONFIG_BROKER_URL_FROM_STDIN
    char line[128];

    if (strcmp(mqtt_cfg.broker.address.uri, "FROM_STDIN") == 0) {
        int count = 0;
        printf("Please enter url of mqtt broker\n");
        while (count < 128) {
            int c = fgetc(stdin);
            if (c == '\n') {
                line[count] = '\0';
                break;
            } else if (c > 0 && c < 127) {
                line[count] = c;
                ++count;
            }
            vTaskDelay(10 / portTICK_PERIOD_MS);
        }
        mqtt_cfg.broker.address.uri = line;
        printf("Broker url: %s\n", line);
    } else {
        ESP_LOGE(TAG, "Configuration mismatch: wrong broker url");
        abort();
    }
#endif /* CONFIG_BROKER_URL_FROM_STDIN */

    auto& client = inst_.client_;
    client = esp_mqtt_client_init(&mqtt_cfg);
    /* The last argument may be used to pass data to the event handler, in this example mqtt_event_handler */
    esp_mqtt_client_register_event(client, (esp_mqtt_event_id_t)ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    esp_mqtt_client_start(client);
}
//------
void mqtt_prepare(void)
{
    esp_log_level_set("*", ESP_LOG_INFO);
    esp_log_level_set("mqtt_client", ESP_LOG_VERBOSE);
    esp_log_level_set("MQTT_EXAMPLE", ESP_LOG_VERBOSE);
    esp_log_level_set("TRANSPORT_BASE", ESP_LOG_VERBOSE);
    esp_log_level_set("esp-tls", ESP_LOG_VERBOSE);
    esp_log_level_set("TRANSPORT", ESP_LOG_VERBOSE);
    esp_log_level_set("outbox", ESP_LOG_VERBOSE);

    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

   

}
//----
void MQTT::setRecvCb(FunRecvCb f)
{ 
    inst_.f_recvCb_ = f; 
}


//----
bool MQTT::connect()
{
    mqtt_prepare();
    mqtt_app_start(cfg_);
    return true;
}
//----
bool MQTT::pub(const string& sTopic, 
                const string& sPayload)
{
    esp_mqtt_client_publish(inst_.client_, 
        sTopic.c_str(), 
        sPayload.c_str(), 0, 1, 0);
    return true;
}
//----
bool MQTT::sub(const string& sTopic)
{
    esp_mqtt_client_subscribe(inst_.client_, 
        sTopic.c_str(), 0);
    return true;
}
//----
bool MQTT::isConnected()const
{
    return inst_.data_.isConnected;
}
MQTT& MQTT::inst() // singleton
    { static MQTT i_; return i_;  }; 