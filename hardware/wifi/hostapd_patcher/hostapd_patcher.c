/*
 * Created by Munjeni @ 2013
 *
 * Tool for Xperia 2012 devies which have bugs related to the hostapd allways unsecured wifi tethering.
 * This tool runing as a service, generate right things and make wifi tethering working with wpa, wpa2 and none.
 * Credits to Morando @ MyCity for his help.
 */

#define LOG_TAG "Hostapd Patcher"

#include <cutils/log.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

#define SOFTAP_CONF "/data/misc/wifi/softap.conf"
#define HOSTAPD_CONF_NEW "/data/misc/wifi/hostapd_xperia.conf"

#define ENC_NONE 0 
#define ENC_WPA  1 
#define ENC_WPA2 4 

extern int __system(const char *command);

int main(void) {
	FILE *pFile, *pFileOut;
	char *ssidBuffer, *passBuffer;
	unsigned short int ssidLen, encType, passLen;
	struct stat fileStat;

	static const char *hostapd_conf[] = {
		"driver=nl80211\n",
		"logger_syslog=-1\n",
		"logger_syslog_level=2\n",
		"logger_stdout=-1\n",
		"logger_stdout_level=2\n",
		"dump_file=/data/misc/wifi/hostapd.dump\n",
		"ctrl_interface=wlan0\n",
		"ctrl_interface_group=wifi\n",
		"hw_mode=g\n",
		"channel=11\n",
		"beacon_int=100\n",
		"dtim_period=2\n",
		"max_num_sta=5\n",
		"supported_rates=10 20 55 110 60 90 120 180 240 360 480 540\n",
		"preamble=1\n",
		"macaddr_acl=0\n",
		"auth_algs=1\n",
		"ignore_broadcast_ssid=0\n",
		"wme_enabled=0\n",
		"eap_server=0\n",
		"own_ip_addr=127.0.0.1\n",
		"interface=wlan0\n",
		"ap_table_max_size=255\n",
		"ap_table_expiration_time=3600\n",
		"tx_queue_data2_aifs=2\n",
		"tx_queue_data2_cwmin=15\n",
		"tx_queue_data2_cwmax=63\n",
		"tx_queue_data2_burst=0\n",
		"interface=wlan0\n"
	};

	size_t count, i;
	count = sizeof(hostapd_conf) / sizeof(hostapd_conf[0]);
	char tmp_buffer[100];

	if (stat(SOFTAP_CONF, &fileStat) < 0) {
		ALOGE("File %s not exist! I must exit now!\n", SOFTAP_CONF);    
		goto done;
	}

	//check if 6.th byte contain sizeof the SSID
	if (fileStat.st_size < 6) {
		ALOGE("File %s not contain what I wanted! I must exit now!\n", SOFTAP_CONF);    
		goto done;
	}		

	if ((pFile = fopen(SOFTAP_CONF, "rb")) == NULL) {
		ALOGE("Fail to open %s for reading! I must exit now!\n", SOFTAP_CONF);
		goto done;
	}

	if ((pFileOut = fopen(HOSTAPD_CONF_NEW, "w")) == NULL) {
		ALOGE("Fail to open %s for writing! I must exit now!\n", HOSTAPD_CONF_NEW);
		goto done;
	}

	// skip to the SSID size atag and read a size of the SSID string
	fseek(pFile, 5, SEEK_SET);
	fread(&ssidLen, 1, 1, pFile);
	if (!ssidLen) {
		ALOGE("Error: there is empty ssid! I must exit now!\n");
		goto fail_done;
	}
	else
		ALOGV("SSID contain %d characters\n", ssidLen);

	// now read SSID string
	ssidBuffer = (char *)malloc(ssidLen+1);
	fread(ssidBuffer, sizeof(char), ssidLen, pFile);
	ssidBuffer[ssidLen] = '\0';
	ALOGV("SSID is: %s\n", ssidBuffer);

	// skip to the encription atag
	fseek(pFile, +3, SEEK_CUR);

	// read encription type
	fread(&encType, 1, 1, pFile);
	if (ENC_NONE == encType) {
		ALOGV("Enc. type is NONE\n");
	} else if(ENC_WPA == encType) {
		ALOGV("Enc. type is WPA\n");
	} else if(ENC_WPA2 == encType) {
		ALOGV("Enc. type is WPA2\n");
	} else {
		ALOGE("Enc. type is UNKNOWN!!! Something is wrong.\n");
		goto free_done;
	}

	// encription things
	if (encType) {
		// skip to the password size atag
		fseek(pFile, +1, SEEK_CUR);

		// read a size of the password string
		fread(&passLen, 1, 1, pFile);
		if (!passLen) {
			ALOGE("Error: there is an empty password! I must exit now!\n");
			goto free_done;
		}
		else
			ALOGV("password contain %d characters\n", passLen);

		// read password
		passBuffer = (char *)malloc(passLen+1);
		fread(passBuffer, sizeof(char), passLen, pFile);
		passBuffer[passLen] = '\0';
		ALOGV("Password is: %s\n", passBuffer);
	}

	// write template
	for (i=0; i<count; ++i)
		fputs(hostapd_conf[i], pFileOut);

	// write security things
	switch(encType) {
		case 4:
			snprintf(tmp_buffer, sizeof(tmp_buffer), "ssid=%s\n", ssidBuffer);
			fputs(tmp_buffer, pFileOut);
			snprintf(tmp_buffer, sizeof(tmp_buffer), "wpa=2\n");
			fputs(tmp_buffer, pFileOut);
			snprintf(tmp_buffer, sizeof(tmp_buffer), "wpa_passphrase=%s\n", passBuffer);
			fputs(tmp_buffer, pFileOut);
			snprintf(tmp_buffer, sizeof(tmp_buffer), "wpa_key_mgmt=WPA-PSK\n");
			fputs(tmp_buffer, pFileOut);
			snprintf(tmp_buffer, sizeof(tmp_buffer), "wpa_pairwise=CCMP\n");
			fputs(tmp_buffer, pFileOut);
			break;
		case 1:
			snprintf(tmp_buffer, sizeof(tmp_buffer), "ssid=%s\n", ssidBuffer);
			fputs(tmp_buffer, pFileOut);
			snprintf(tmp_buffer, sizeof(tmp_buffer), "wpa=1\n");
			fputs(tmp_buffer, pFileOut);
			snprintf(tmp_buffer, sizeof(tmp_buffer), "wpa_passphrase=%s\n", passBuffer);
			fputs(tmp_buffer, pFileOut);
			snprintf(tmp_buffer, sizeof(tmp_buffer), "wpa_key_mgmt=WPA-PSK\n");
			fputs(tmp_buffer, pFileOut);
			snprintf(tmp_buffer, sizeof(tmp_buffer), "wpa_pairwise=CCMP\n");
			fputs(tmp_buffer, pFileOut);
			break;
		default:
			snprintf(tmp_buffer, sizeof(tmp_buffer), "ssid=%s\n", ssidBuffer);
			fputs(tmp_buffer, pFileOut);
			snprintf(tmp_buffer, sizeof(tmp_buffer), "wpa=0\n");
			fputs(tmp_buffer, pFileOut);
			snprintf(tmp_buffer, sizeof(tmp_buffer), "wep_rekey_period=0\n");
			fputs(tmp_buffer, pFileOut);
			snprintf(tmp_buffer, sizeof(tmp_buffer), "wpa_group_rekey=0\n");
			fputs(tmp_buffer, pFileOut);
			snprintf(tmp_buffer, sizeof(tmp_buffer), "wpa_gmk_rekey=0\n");
			fputs(tmp_buffer, pFileOut);
			snprintf(tmp_buffer, sizeof(tmp_buffer), "wpa_ptk_rekey=0\n");
			fputs(tmp_buffer, pFileOut);
			break;
	}

	// give propper perms
	__system("chmod 0660 /data/misc/wifi/hostapd_xperia.conf");
	__system("chown wifi:wifi /data/misc/wifi/hostapd_xperia.conf");

free_done:
	free(ssidBuffer);
	if (encType && passLen)
		free(passBuffer);

fail_done:
	fclose(pFile);
	fclose(pFileOut);

done:
	return 0;
}
