#include <stdio.h>
#include <stdint.h>
#include <pcap.h>
#include <arpa/inet.h>
#include "airodump-ng.h"

struct radiotap_header {
	u_int8_t it_version; //radiotap header version, present this value always 0
	u_int8_t it_pad; //currently unused, simply aligns the fiels
	u_int16_t it_len; //radiotap entire length value
	u_int32_t it_present; //bitmask of the radiotap data fields
	u_int64_t mactime; //TSFT
	u_int8_t flags;
	u_int8_t rate;
	u_int16_t channel_frequency, chanel_flags;
	int8_t antenna_signal;
	u_int8_t antenna;
	u_int16_t rx_flags;
} __attribute__((__packed__)); //command full pack


struct ieee80211_header{
	u_int8_t FC_type_subtype;
	u_int8_t FC_flags;
	u_int16_t duration;
	u_int8_t receiver_addr[6] //destination_addr
	u_int8_t transmitter_addr[6]; //source_addr
       	u_int8_t bssid_addr[6];
	u_int16_t sequence_control;
};

struct type_subtype{
	u_char subtype : 4;
	u_char type : 2;
	u_char version : 2;
};
/*
struct flags{
	u_int64_t mactime; //TSFT, 0
	u_int8_t flags; //1
	u_int8_t rate; //2
	u_int16_t channel_frequency, channel_flags; //3
	u_int8_t hop_set, hop_pattern; //FHSS, 4
	int8_t antenna_signal; //5
	int8_t antenna_noise; //6
	u_int16_t lock_quality; //7
	u_int16_t TX_attenuation; //8
	u_int16_t db_TX_attenuation; //9
	int8_t dBm_TX_power; //10
	u_int8_t antenna; //11
	u_int8_t db_antenna_signal; //12
	u_int8_t db_antenna_noise; //13
	u_int16_t rx_flags; //14
	//15~18
	u_int8_t known, flags, mcs; // MCS, 19
	u_int32_t reference_number; //A-MPDU status, 20
	u_int16_t flags;
	u_int8_t delimiter_CRC_value, reserved;
	u_int16_t known; //VHT, 21
	u_int8_t flags, bandwidth, mcs_nss[4], coding, group_id;
	u_int16_t partial_aid;
	u_int64_t timestamp; //timestamp, 22
	u_int16_t accuracy;
	u_int8_t unit, position, flags;
	//23~28
	//radiotap namespace, unused, 29
	u_int8_t OUT[3], sub_namespace; //vendor namespace, 30
	u_int16_t skip_length; 
}*/


void printMAC(u_char *mac){
	for(int i=0;i<6;i++)
		printf("%02X:", mac[i]);
	printf("\b");

}



int main(int argc, char *argv[]){
	char *dev;
	char errbuf[PCAP_ERRBUF_SIZE];

	pcap_t *handle = NULL;
	struct pcap_pkthdr *header;
	const u_char * packet;
	struct radiotap_header * radiotap;
	struct ieee80211_header * ieee80211;
	int mode;

	if(argc == 2)
		handle = pcap_open_live(argv[1], BUFSIZ, 1, 1000, errbuf);
	else{
		printf("Plz input \"myairodump [device]\"\n");
		return -1;
	}

	while(1){
		mode = pcap_next_ex(handle, &header, &packet);
		
		if(mdoe==0)
			continue;
		else if(mode==-1)
			break;
		else if(mode==-2){
			printf("EOF\n");
			break;
		}
		
		radiotap = (struct radiotap_header *)packet;


		ieee80211 = (struct ieee80211_header *)(packet+radiotap->it_len);
		u_char type;
		struct type_subtype * type;  // 0000(subtype) 00(type) 00(version)
		type = (struct type_subtype *)ieee80211->FC_type_subtype;
		
		printf("Frequency : [ch : %d ]\n" (radiotap->channel_frequency-2407)/5);
		
	}
	return 0;
}
