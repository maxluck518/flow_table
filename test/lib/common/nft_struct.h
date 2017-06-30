struct nf2device {
	char *device_name;
	int fd;
	int net_iface;
    char server_ip_addr[MAX_IPADDR_LEN];
    int server_port_num;
};