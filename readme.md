## flow entry agent
### introduction
#### five tables
        *   qinq
        *   domain
        *   L3
        *   pppL2
        *   extL2
        *   mytable
#### table phy_address
        *   qinq        :       0x43d40000
        *   domain      :       0x43d00000
        *   L3          :       0x43d20000
        *   pppL2       :       0x43d30000
        *   extL2       :       0x43d10000
        *   mytable     :       0x43e00000
#### testcase
        *   case1       :       pppoe,ipv4
        *   case2       :       arp
        *   case3       :       icmp
#### usage
        *   ./test/icmp.txt     :       flow_entry example (case1,2,3)
        *   ./test/app          :       app for flow_entry
### shell commands structure
    | 0         |1       |2         |3           |4             |5              |6               |7             |8
    | table_add | qinq   | priority | ethertype  | pppoe_code   | mask_etype    | mask_pcode     | qinq_ac_list
    | table_add | domain | priority | ethertype  | ppp_protocol | mask_etype    | mask_pproto    | domain_ac_list
    | table_add | L3     | priority | dst_ip     | mask         | l3_ac_list    | dmac           | smac         | sid
    | table_add | pppL2  | priority | dmac       | mask         | pppl2_ac_list | port           | svid         | cvid
    | table_add | extL2  | priority | dmac       | mask         | extl2_ac_list | port           | svid         | cvid
    | table_add | mytable  | priority | phy_src_port | fivetuple | mask         | phy_dst_port

### example

    | table_add | L3    | 20 | 10101010     | ffffffff     | 4    | aabbccddeeff | 112233445566 | 8888
    | table_add | pppL2 | 1f | 123456123456 | ffffffffffff | 5    | 2            | 001          | 002
    | table_add | qinq  | 23 | 88a8         | 09           | ffff | ff           | 0
    | table_add | mytable   | 1 | 01 | 00000000 | 00000000 | 00 | 0000 | 0000 | ff | 00000000 | 00000000 | 00 | 0000 | 0000 | 02 |

