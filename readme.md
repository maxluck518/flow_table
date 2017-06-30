### flow entry agent
#### introduction
    ##### five tables
        *   qinq
        *   domain
        *   L3
        *   pppL2
        *   extL2
    ##### action lists
        To be continued!
#### shell commands structure

    | table_add | qinq   | priority | ethertype  | pppoe_code   | mask          | qinq_ac_list
    | table_add | domain | priority | ethertype  | ppp_protocol | mask          | domain_ac_list
    | table_add | L3     | priority | dst_ip     | mask         | l3_ac_list    | dmac           | smac         | sid
    | table_add | pppL2  | priority | dmac       | mask         | pppl2_ac_list | port           | svid         | cvid
    | table_add | extL2  | priority | dmac       | mask         | extl2_ac_list | port           | svid         | cvid

#### example

    | table_add | L3    | 20 | 10101010     | ffffffff     | 4 | aabbccddeeff | 112233445566 | 8888
    | table_add | pppL2 | 1f | 123456123456 | ffffffffffff | 5 | 2            | 001          | 002
