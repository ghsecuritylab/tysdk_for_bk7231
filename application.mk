
# Initialize tool chain  /usr/bin/gcc-arm-none-eabi-4_9-2015q1/bin
# -------------------------------------------------------------------
ARM_GCC_TOOLCHAIN = toolchain/gcc-arm-none-eabi-4_9-2015q1/bin

CROSS_COMPILE = $(ARM_GCC_TOOLCHAIN)/arm-none-eabi-

ota_idx = 1

# Compilation tools
AR = $(CROSS_COMPILE)ar
CC = $(CROSS_COMPILE)gcc
AS = $(CROSS_COMPILE)as
NM = $(CROSS_COMPILE)nm
LD = $(CROSS_COMPILE)gcc
GDB = $(CROSS_COMPILE)gdb
OBJCOPY = $(CROSS_COMPILE)objcopy
OBJDUMP = $(CROSS_COMPILE)objdump

OS := $(shell uname)

ifeq ($(findstring CYGWIN, $(OS)), CYGWIN) 
PICK = $(AMEBA_TOOLDIR)pick.exe
PAD  = $(AMEBA_TOOLDIR)padding.exe
CHKSUM = $(AMEBA_TOOLDIR)checksum.exe
OTA = $(AMEBA_TOOLDIR)ota.exe
else
PICK = $(AMEBA_TOOLDIR)pick
PAD  = $(AMEBA_TOOLDIR)padding
CHKSUM = $(AMEBA_TOOLDIR)checksum
OTA = $(AMEBA_TOOLDIR)ota
endif

# -------------------------------------------------------------------
# Initialize target name and target object files
# -------------------------------------------------------------------

all: application 

TARGET=Debug

OBJ_DIR=$(TARGET)/obj
BIN_DIR=$(TARGET)/bin

# -------------------------------------------------------------------
# Include folder list
# -------------------------------------------------------------------
INCLUDES =

INCLUDES += -I./beken378/common
INCLUDES += -I./beken378/app
INCLUDES += -I./beken378/app/config
INCLUDES += -I./beken378/app/standalone-station
INCLUDES += -I./beken378/app/standalone-ap
INCLUDES += -I./beken378/ip/common
INCLUDES += -I./beken378/ip/ke/
INCLUDES += -I./beken378/ip/mac/
INCLUDES += -I./beken378/ip/lmac/src/hal
INCLUDES += -I./beken378/ip/lmac/src/mm
INCLUDES += -I./beken378/ip/lmac/src/ps
INCLUDES += -I./beken378/ip/lmac/src/rd
INCLUDES += -I./beken378/ip/lmac/src/rwnx
INCLUDES += -I./beken378/ip/lmac/src/rx
INCLUDES += -I./beken378/ip/lmac/src/scan
INCLUDES += -I./beken378/ip/lmac/src/sta
INCLUDES += -I./beken378/ip/lmac/src/tx
INCLUDES += -I./beken378/ip/lmac/src/vif
INCLUDES += -I./beken378/ip/lmac/src/rx/rxl
INCLUDES += -I./beken378/ip/lmac/src/tx/txl
INCLUDES += -I./beken378/ip/lmac/src/p2p
INCLUDES += -I./beken378/ip/lmac/src/chan
INCLUDES += -I./beken378/ip/lmac/src/td
INCLUDES += -I./beken378/ip/lmac/src/tpc
INCLUDES += -I./beken378/ip/lmac/src/tdls
INCLUDES += -I./beken378/ip/umac/src/mesh
INCLUDES += -I./beken378/ip/umac/src/rc
INCLUDES += -I./beken378/ip/umac/src/apm
INCLUDES += -I./beken378/ip/umac/src/bam
INCLUDES += -I./beken378/ip/umac/src/llc
INCLUDES += -I./beken378/ip/umac/src/me
INCLUDES += -I./beken378/ip/umac/src/rxu
INCLUDES += -I./beken378/ip/umac/src/scanu
INCLUDES += -I./beken378/ip/umac/src/sm
INCLUDES += -I./beken378/ip/umac/src/txu
INCLUDES += -I./beken378/driver/include
INCLUDES += -I./beken378/driver/common/reg
INCLUDES += -I./beken378/driver/entry
INCLUDES += -I./beken378/driver/dma
INCLUDES += -I./beken378/driver/intc
INCLUDES += -I./beken378/driver/phy
INCLUDES += -I./beken378/driver/rc_beken
INCLUDES += -I./beken378/driver/flash
INCLUDES += -I./beken378/driver/rw_pub
INCLUDES += -I./beken378/driver/common/reg
INCLUDES += -I./beken378/driver/common
INCLUDES += -I./beken378/driver/uart
INCLUDES += -I./beken378/driver/sys_ctrl
INCLUDES += -I./beken378/driver/gpio
INCLUDES += -I./beken378/driver/general_dma
INCLUDES += -I./beken378/driver/spidma
INCLUDES += -I./beken378/driver/icu
INCLUDES += -I./beken378/func/include
INCLUDES += -I./beken378/func/rf_test
INCLUDES += -I./beken378/func/user_driver
INCLUDES += -I./beken378/func/power_save
INCLUDES += -I./beken378/func/uart_debug
INCLUDES += -I./beken378/func/ethernet_intf
INCLUDES += -I./beken378/func/hostapd-2.5/hostapd
INCLUDES += -I./beken378/func/hostapd-2.5/bk_patch
INCLUDES += -I./beken378/func/hostapd-2.5/src/utils
INCLUDES += -I./beken378/func/hostapd-2.5/src/ap
INCLUDES += -I./beken378/func/hostapd-2.5/src/common
INCLUDES += -I./beken378/func/hostapd-2.5/src/drivers
INCLUDES += -I./beken378/func/hostapd-2.5/src
INCLUDES += -I./beken378/func/lwip_intf/lwip-2.0.2/src
INCLUDES += -I./beken378/func/lwip_intf/lwip-2.0.2/port
INCLUDES += -I./beken378/func/lwip_intf/lwip-2.0.2/src/include
INCLUDES += -I./beken378/func/lwip_intf/lwip-2.0.2/src/include/netif
INCLUDES += -I./beken378/func/lwip_intf/lwip-2.0.2/src/include/lwip
INCLUDES += -I./beken378/func/temp_detect
INCLUDES += -I./beken378/func/spidma_intf
INCLUDES += -I./beken378/func/rwnx_intf
INCLUDES += -I./beken378/func/joint_up
INCLUDES += -I./beken378/os/include
INCLUDES += -I./beken378/os/FreeRTOSv9.0.0/FreeRTOS/Source/portable/Keil/ARM968es
INCLUDES += -I./beken378/os/FreeRTOSv9.0.0/FreeRTOS/Source/include
INCLUDES += -I./beken378/os/FreeRTOSv9.0.0


# -------------------------------------------------------------------
# Source file list
# -------------------------------------------------------------------
SRC_C =
DRAM_C =

#application layer
SRC_C += ./beken378/app/app.c
SRC_C += ./beken378/app/ate_app.c
SRC_C += ./beken378/app/config/param_config.c
SRC_C += ./beken378/app/standalone-ap/sa_ap.c
SRC_C += ./beken378/app/standalone-station/sa_station.c

#demo module
SRC_C += ./beken378/demo/ieee802_11_demo.c

#driver layer
SRC_C += ./beken378/driver/common/dd.c
SRC_C += ./beken378/driver/common/drv_model.c
SRC_C += ./beken378/driver/dma/dma.c
SRC_C += ./beken378/driver/driver.c
SRC_C += ./beken378/driver/entry/arch_main.c
SRC_C += ./beken378/driver/fft/fft.c
SRC_C += ./beken378/driver/flash/flash.c
SRC_C += ./beken378/driver/general_dma/general_dma.c
SRC_C += ./beken378/driver/gpio/gpio.c
SRC_C += ./beken378/driver/i2s/i2s.c
SRC_C += ./beken378/driver/icu/icu.c
SRC_C += ./beken378/driver/intc/intc.c
SRC_C += ./beken378/driver/irda/irda.c
SRC_C += ./beken378/driver/macphy_bypass/mac_phy_bypass.c
SRC_C += ./beken378/driver/phy/phy_trident.c
SRC_C += ./beken378/driver/pwm/pwm.c
SRC_C += ./beken378/driver/pwm/mcu_ps_timer.c
SRC_C += ./beken378/driver/pwm/bk_timer.c
SRC_C += ./beken378/driver/rw_pub/rw_platf_pub.c
SRC_C += ./beken378/driver/saradc/saradc.c
SRC_C += ./beken378/driver/spi/spi.c
SRC_C += ./beken378/driver/spidma/spidma.c
SRC_C += ./beken378/driver/sys_ctrl/sys_ctrl.c
SRC_C += ./beken378/driver/uart/Retarget.c
SRC_C += ./beken378/driver/uart/uart.c
SRC_C += ./beken378/driver/wdt/wdt.c

#function layer
SRC_C += ./beken378/func/func.c
SRC_C += ./beken378/func/bk7011_cal/bk7231_cal.c
SRC_C += ./beken378/func/bk7011_cal/manual_cal_bk7231.c
SRC_C += ./beken378/func/joint_up/role_launch.c
SRC_C += ./beken378/func/joint_up/reconnect_startup.c
SRC_C += ./beken378/func/hostapd_intf/hostapd_intf.c
SRC_C += ./beken378/func/hostapd-2.5/bk_patch/ddrv.c
SRC_C += ./beken378/func/hostapd-2.5/bk_patch/signal.c
SRC_C += ./beken378/func/hostapd-2.5/bk_patch/sk_intf.c
SRC_C += ./beken378/func/hostapd-2.5/bk_patch/fake_socket.c
SRC_C += ./beken378/func/hostapd-2.5/hostapd/main_none.c
SRC_C += ./beken378/func/hostapd-2.5/src/crypto/aes-internal.c
SRC_C += ./beken378/func/hostapd-2.5/src/crypto/aes-internal-dec.c
SRC_C += ./beken378/func/hostapd-2.5/src/crypto/aes-internal-enc.c
SRC_C += ./beken378/func/hostapd-2.5/src/crypto/aes-unwrap.c
SRC_C += ./beken378/func/hostapd-2.5/src/crypto/aes-wrap.c
SRC_C += ./beken378/func/hostapd-2.5/src/crypto/bk_md5.c
SRC_C += ./beken378/func/hostapd-2.5/src/crypto/md5-internal.c
SRC_C += ./beken378/func/hostapd-2.5/src/crypto/rc4.c
SRC_C += ./beken378/func/hostapd-2.5/src/crypto/bk_sha1.c
SRC_C += ./beken378/func/hostapd-2.5/src/crypto/sha1-internal.c
SRC_C += ./beken378/func/hostapd-2.5/src/crypto/sha1-pbkdf2.c
SRC_C += ./beken378/func/hostapd-2.5/src/crypto/sha1-prf.c
SRC_C += ./beken378/func/hostapd-2.5/src/crypto/tls_none.c
SRC_C += ./beken378/func/hostapd-2.5/src/ap/ap_config.c
SRC_C += ./beken378/func/hostapd-2.5/src/ap/ap_drv_ops.c
SRC_C += ./beken378/func/hostapd-2.5/src/ap/ap_list.c
SRC_C += ./beken378/func/hostapd-2.5/src/ap/ap_mlme.c
SRC_C += ./beken378/func/hostapd-2.5/src/ap/authsrv.c
SRC_C += ./beken378/func/hostapd-2.5/src/ap/beacon.c
SRC_C += ./beken378/func/hostapd-2.5/src/ap/bss_load.c
SRC_C += ./beken378/func/hostapd-2.5/src/ap/dfs.c
SRC_C += ./beken378/func/hostapd-2.5/src/ap/drv_callbacks.c
SRC_C += ./beken378/func/hostapd-2.5/src/ap/eap_user_db.c
SRC_C += ./beken378/func/hostapd-2.5/src/ap/hostapd.c
SRC_C += ./beken378/func/hostapd-2.5/src/ap/hw_features.c
SRC_C += ./beken378/func/hostapd-2.5/src/ap/ieee802_11_auth.c
SRC_C += ./beken378/func/hostapd-2.5/src/ap/ieee802_11.c
SRC_C += ./beken378/func/hostapd-2.5/src/ap/ieee802_11_ht.c
SRC_C += ./beken378/func/hostapd-2.5/src/ap/ieee802_11_shared.c
SRC_C += ./beken378/func/hostapd-2.5/src/ap/ieee802_1x.c
SRC_C += ./beken378/func/hostapd-2.5/src/ap/pmksa_cache_auth.c
SRC_C += ./beken378/func/hostapd-2.5/src/ap/sta_info.c
SRC_C += ./beken378/func/hostapd-2.5/src/ap/tkip_countermeasures.c
SRC_C += ./beken378/func/hostapd-2.5/src/ap/utils.c
SRC_C += ./beken378/func/hostapd-2.5/src/ap/wmm.c
SRC_C += ./beken378/func/hostapd-2.5/src/ap/wpa_auth.c
SRC_C += ./beken378/func/hostapd-2.5/src/ap/wpa_auth_glue.c
SRC_C += ./beken378/func/hostapd-2.5/src/ap/wpa_auth_ie.c
SRC_C += ./beken378/func/hostapd-2.5/src/common/hw_features_common.c
SRC_C += ./beken378/func/hostapd-2.5/src/common/ieee802_11_common.c
SRC_C += ./beken378/func/hostapd-2.5/src/common/wpa_common.c
SRC_C += ./beken378/func/hostapd-2.5/src/drivers/driver_beken.c
SRC_C += ./beken378/func/hostapd-2.5/src/drivers/driver_common.c
SRC_C += ./beken378/func/hostapd-2.5/src/drivers/drivers.c
SRC_C += ./beken378/func/hostapd-2.5/src/eap_common/eap_common.c
SRC_C += ./beken378/func/hostapd-2.5/src/eapol_auth/eapol_auth_sm.c
SRC_C += ./beken378/func/hostapd-2.5/src/eap_server/eap_server.c
SRC_C += ./beken378/func/hostapd-2.5/src/eap_server/eap_server_methods.c
SRC_C += ./beken378/func/hostapd-2.5/src/l2_packet/l2_packet_none.c
SRC_C += ./beken378/func/hostapd-2.5/src/rsn_supp/preauth.c
SRC_C += ./beken378/func/hostapd-2.5/src/rsn_supp/wpa.c
SRC_C += ./beken378/func/hostapd-2.5/src/rsn_supp/wpa_ie.c
SRC_C += ./beken378/func/hostapd-2.5/src/utils/common.c
SRC_C += ./beken378/func/hostapd-2.5/src/utils/eloop.c
SRC_C += ./beken378/func/hostapd-2.5/src/utils/os_none.c
SRC_C += ./beken378/func/hostapd-2.5/src/utils/wpabuf.c
SRC_C += ./beken378/func/hostapd-2.5/wpa_supplicant/ap.c
SRC_C += ./beken378/func/hostapd-2.5/wpa_supplicant/blacklist.c
SRC_C += ./beken378/func/hostapd-2.5/wpa_supplicant/bss.c
SRC_C += ./beken378/func/hostapd-2.5/wpa_supplicant/config.c
SRC_C += ./beken378/func/hostapd-2.5/wpa_supplicant/config_none.c
SRC_C += ./beken378/func/hostapd-2.5/wpa_supplicant/eap_register.c
SRC_C += ./beken378/func/hostapd-2.5/wpa_supplicant/events.c
SRC_C += ./beken378/func/hostapd-2.5/wpa_supplicant/main_supplicant.c
SRC_C += ./beken378/func/hostapd-2.5/wpa_supplicant/notify.c
SRC_C += ./beken378/func/hostapd-2.5/wpa_supplicant/wmm_ac.c
SRC_C += ./beken378/func/hostapd-2.5/wpa_supplicant/wpa_scan.c
SRC_C += ./beken378/func/hostapd-2.5/wpa_supplicant/wpas_glue.c
SRC_C += ./beken378/func/hostapd-2.5/wpa_supplicant/wpa_supplicant.c
SRC_C += ./beken378/func/lwip_intf/lwip-2.0.2/port/ethernetif.c
SRC_C += ./beken378/func/lwip_intf/lwip-2.0.2/port/net.c
SRC_C += ./beken378/func/lwip_intf/lwip-2.0.2/port/sys_arch.c
SRC_C += ./beken378/func/lwip_intf/lwip-2.0.2/src/api/api_lib.c
SRC_C += ./beken378/func/lwip_intf/lwip-2.0.2/src/api/api_msg.c
SRC_C += ./beken378/func/lwip_intf/lwip-2.0.2/src/api/err.c
SRC_C += ./beken378/func/lwip_intf/lwip-2.0.2/src/api/netbuf.c
SRC_C += ./beken378/func/lwip_intf/lwip-2.0.2/src/api/netdb.c
SRC_C += ./beken378/func/lwip_intf/lwip-2.0.2/src/api/netifapi.c
SRC_C += ./beken378/func/lwip_intf/lwip-2.0.2/src/api/sockets.c
SRC_C += ./beken378/func/lwip_intf/lwip-2.0.2/src/api/tcpip.c
SRC_C += ./beken378/func/lwip_intf/lwip-2.0.2/src/core/def.c
SRC_C += ./beken378/func/lwip_intf/lwip-2.0.2/src/core/dns.c
SRC_C += ./beken378/func/lwip_intf/lwip-2.0.2/src/core/inet_chksum.c
SRC_C += ./beken378/func/lwip_intf/lwip-2.0.2/src/core/init.c
SRC_C += ./beken378/func/lwip_intf/lwip-2.0.2/src/core/ip.c
SRC_C += ./beken378/func/lwip_intf/lwip-2.0.2/src/core/ipv4/autoip.c
SRC_C += ./beken378/func/lwip_intf/lwip-2.0.2/src/core/ipv4/dhcp.c
SRC_C += ./beken378/func/lwip_intf/lwip-2.0.2/src/core/ipv4/etharp.c
SRC_C += ./beken378/func/lwip_intf/lwip-2.0.2/src/core/ipv4/icmp.c
SRC_C += ./beken378/func/lwip_intf/lwip-2.0.2/src/core/ipv4/igmp.c
SRC_C += ./beken378/func/lwip_intf/lwip-2.0.2/src/core/ipv4/ip4_addr.c
SRC_C += ./beken378/func/lwip_intf/lwip-2.0.2/src/core/ipv4/ip4.c
SRC_C += ./beken378/func/lwip_intf/lwip-2.0.2/src/core/ipv4/ip4_frag.c
SRC_C += ./beken378/func/lwip_intf/lwip-2.0.2/src/core/ipv6/dhcp6.c
SRC_C += ./beken378/func/lwip_intf/lwip-2.0.2/src/core/ipv6/ethip6.c
SRC_C += ./beken378/func/lwip_intf/lwip-2.0.2/src/core/ipv6/icmp6.c
SRC_C += ./beken378/func/lwip_intf/lwip-2.0.2/src/core/ipv6/inet6.c
SRC_C += ./beken378/func/lwip_intf/lwip-2.0.2/src/core/ipv6/ip6_addr.c
SRC_C += ./beken378/func/lwip_intf/lwip-2.0.2/src/core/ipv6/ip6.c
SRC_C += ./beken378/func/lwip_intf/lwip-2.0.2/src/core/ipv6/ip6_frag.c
SRC_C += ./beken378/func/lwip_intf/lwip-2.0.2/src/core/ipv6/mld6.c
SRC_C += ./beken378/func/lwip_intf/lwip-2.0.2/src/core/ipv6/nd6.c
SRC_C += ./beken378/func/lwip_intf/lwip-2.0.2/src/core/mem.c
SRC_C += ./beken378/func/lwip_intf/lwip-2.0.2/src/core/memp.c
SRC_C += ./beken378/func/lwip_intf/lwip-2.0.2/src/core/netif.c
SRC_C += ./beken378/func/lwip_intf/lwip-2.0.2/src/core/pbuf.c
SRC_C += ./beken378/func/lwip_intf/lwip-2.0.2/src/core/raw.c
SRC_C += ./beken378/func/lwip_intf/lwip-2.0.2/src/core/stats.c
SRC_C += ./beken378/func/lwip_intf/lwip-2.0.2/src/core/sys.c
SRC_C += ./beken378/func/lwip_intf/lwip-2.0.2/src/core/tcp.c
SRC_C += ./beken378/func/lwip_intf/lwip-2.0.2/src/core/tcp_in.c
SRC_C += ./beken378/func/lwip_intf/lwip-2.0.2/src/core/tcp_out.c
SRC_C += ./beken378/func/lwip_intf/lwip-2.0.2/src/core/timeouts.c
SRC_C += ./beken378/func/lwip_intf/lwip-2.0.2/src/core/udp.c
SRC_C += ./beken378/func/lwip_intf/lwip-2.0.2/src/netif/ethernet.c
SRC_C += ./beken378/func/lwip_intf/dhcpd/dhcp-server.c
SRC_C += ./beken378/func/lwip_intf/dhcpd/dhcp-server-main.c
SRC_C += ./beken378/func/misc/fake_clock.c
SRC_C += ./beken378/func/misc/target_util.c
SRC_C += ./beken378/func/power_save/power_save.c
SRC_C += ./beken378/func/power_save/manual_ps.c
SRC_C += ./beken378/func/power_save/mcu_ps.c
SRC_C += ./beken378/func/rf_test/rx_sensitivity.c
SRC_C += ./beken378/func/rf_test/tx_evm.c
SRC_C += ./beken378/func/rwnx_intf/rw_ieee80211.c
SRC_C += ./beken378/func/rwnx_intf/rw_msdu.c
SRC_C += ./beken378/func/rwnx_intf/rw_msg_rx.c
SRC_C += ./beken378/func/rwnx_intf/rw_msg_tx.c
SRC_C += ./beken378/func/sim_uart/gpio_uart.c
SRC_C += ./beken378/func/sim_uart/pwm_uart.c
SRC_C += ./beken378/func/spidma_intf/spidma_intf.c
SRC_C += ./beken378/func/temp_detect/temp_detect.c
SRC_C += ./beken378/func/uart_debug/cmd_evm.c
SRC_C += ./beken378/func/uart_debug/cmd_help.c
SRC_C += ./beken378/func/uart_debug/cmd_reg.c
SRC_C += ./beken378/func/uart_debug/cmd_rx_sensitivity.c
SRC_C += ./beken378/func/uart_debug/command_line.c
SRC_C += ./beken378/func/uart_debug/command_table.c
SRC_C += ./beken378/func/uart_debug/udebug.c
SRC_C += ./beken378/func/user_driver/BkDriverFlash.c
SRC_C += ./beken378/func/user_driver/BkDriverGpio.c
SRC_C += ./beken378/func/user_driver/BkDriverPwm.c
SRC_C += ./beken378/func/user_driver/BkDriverUart.c
SRC_C += ./beken378/func/user_driver/BkDriverWdg.c
SRC_C += ./beken378/func/wlan_ui/wlan_cli.c
SRC_C += ./beken378/func/wlan_ui/wlan_ui.c

#rwnx ip module
#SRC_C += ./beken378/ip/common/co_dlist.c
#SRC_C += ./beken378/ip/common/co_list.c
#SRC_C += ./beken378/ip/common/co_math.c
#SRC_C += ./beken378/ip/common/co_pool.c
#SRC_C += ./beken378/ip/common/co_ring.c
#SRC_C += ./beken378/ip/ke/ke_env.c
#SRC_C += ./beken378/ip/ke/ke_event.c
#SRC_C += ./beken378/ip/ke/ke_msg.c
#SRC_C += ./beken378/ip/ke/ke_queue.c
#SRC_C += ./beken378/ip/ke/ke_task.c
#SRC_C += ./beken378/ip/ke/ke_timer.c
#SRC_C += ./beken378/ip/lmac/src/chan/chan.c
#SRC_C += ./beken378/ip/lmac/src/hal/hal_desc.c
#SRC_C += ./beken378/ip/lmac/src/hal/hal_dma.c
#SRC_C += ./beken378/ip/lmac/src/hal/hal_machw.c
#SRC_C += ./beken378/ip/lmac/src/hal/hal_mib.c
#SRC_C += ./beken378/ip/lmac/src/mm/mm.c
#SRC_C += ./beken378/ip/lmac/src/mm/mm_bcn.c
#SRC_C += ./beken378/ip/lmac/src/mm/mm_task.c
#SRC_C += ./beken378/ip/lmac/src/mm/mm_timer.c
#SRC_C += ./beken378/ip/lmac/src/p2p/p2p.c
#SRC_C += ./beken378/ip/lmac/src/ps/ps.c
#SRC_C += ./beken378/ip/lmac/src/rd/rd.c
#SRC_C += ./beken378/ip/lmac/src/rwnx/rwnx.c
#SRC_C += ./beken378/ip/lmac/src/rx/rx_swdesc.c
#SRC_C += ./beken378/ip/lmac/src/rx/rxl/rxl_cntrl.c
#SRC_C += ./beken378/ip/lmac/src/rx/rxl/rxl_hwdesc.c
#SRC_C += ./beken378/ip/lmac/src/scan/scan.c
#SRC_C += ./beken378/ip/lmac/src/scan/scan_shared.c
#SRC_C += ./beken378/ip/lmac/src/scan/scan_task.c
#SRC_C += ./beken378/ip/lmac/src/sta/sta_mgmt.c
#SRC_C += ./beken378/ip/lmac/src/td/td.c
#SRC_C += ./beken378/ip/lmac/src/tdls/tdls.c
#SRC_C += ./beken378/ip/lmac/src/tdls/tdls_task.c
#SRC_C += ./beken378/ip/lmac/src/tpc/tpc.c
#SRC_C += ./beken378/ip/lmac/src/tx/tx_swdesc.c
#SRC_C += ./beken378/ip/lmac/src/tx/txl/txl_buffer.c
#SRC_C += ./beken378/ip/lmac/src/tx/txl/txl_buffer_shared.c
#SRC_C += ./beken378/ip/lmac/src/tx/txl/txl_cfm.c
#SRC_C += ./beken378/ip/lmac/src/tx/txl/txl_cntrl.c
#SRC_C += ./beken378/ip/lmac/src/tx/txl/txl_frame.c
#SRC_C += ./beken378/ip/lmac/src/tx/txl/txl_frame_shared.c
#SRC_C += ./beken378/ip/lmac/src/tx/txl/txl_hwdesc.c
#SRC_C += ./beken378/ip/lmac/src/vif/vif_mgmt.c
#SRC_C += ./beken378/ip/mac/mac.c
#SRC_C += ./beken378/ip/mac/mac_ie.c
#SRC_C += ./beken378/ip/umac/src/apm/apm.c
#SRC_C += ./beken378/ip/umac/src/apm/apm_task.c
#SRC_C += ./beken378/ip/umac/src/bam/bam.c
#SRC_C += ./beken378/ip/umac/src/bam/bam_task.c
#SRC_C += ./beken378/ip/umac/src/me/me.c
#SRC_C += ./beken378/ip/umac/src/me/me_mgmtframe.c
#SRC_C += ./beken378/ip/umac/src/me/me_mic.c
#SRC_C += ./beken378/ip/umac/src/me/me_task.c
#SRC_C += ./beken378/ip/umac/src/me/me_utils.c
#SRC_C += ./beken378/ip/umac/src/rc/rc.c
#SRC_C += ./beken378/ip/umac/src/rc/rc_basic.c
#SRC_C += ./beken378/ip/umac/src/rxu/rxu_cntrl.c
#SRC_C += ./beken378/ip/umac/src/scanu/scanu.c
#SRC_C += ./beken378/ip/umac/src/scanu/scanu_shared.c
#SRC_C += ./beken378/ip/umac/src/scanu/scanu_task.c
#SRC_C += ./beken378/ip/umac/src/sm/sm.c
#SRC_C += ./beken378/ip/umac/src/sm/sm_task.c
#SRC_C += ./beken378/ip/umac/src/txu/txu_cntrl.c 

#operation system module
SRC_C += ./beken378/os/FreeRTOSv9.0.0/FreeRTOS/Source/croutine.c
SRC_C += ./beken378/os/FreeRTOSv9.0.0/FreeRTOS/Source/event_groups.c
SRC_C += ./beken378/os/FreeRTOSv9.0.0/FreeRTOS/Source/list.c
SRC_C += ./beken378/os/FreeRTOSv9.0.0/FreeRTOS/Source/portable/Keil/ARM968es/port.c
SRC_C += ./beken378/os/FreeRTOSv9.0.0/FreeRTOS/Source/portable/MemMang/heap_4.c
SRC_C += ./beken378/os/FreeRTOSv9.0.0/FreeRTOS/Source/queue.c
SRC_C += ./beken378/os/FreeRTOSv9.0.0/FreeRTOS/Source/tasks.c
SRC_C += ./beken378/os/FreeRTOSv9.0.0/FreeRTOS/Source/timers.c
SRC_C += ./beken378/os/FreeRTOSv9.0.0/rtos_pub.c
SRC_C += ./beken378/os/mem_arch.c
SRC_C += ./beken378/os/str_arch.c

#examples for customer

#assembling files
SRC_S = 
SRC_S +=  ./beken378/driver/entry/boot_handlers.S
SRC_S +=  ./beken378/driver/entry/boot_vectors.S

# Generate obj list
# -------------------------------------------------------------------
SRC_O = $(patsubst %.c,%.o,$(SRC_C))
SRC_C_LIST = $(notdir $(SRC_C)) $(notdir $(DRAM_C))
OBJ_LIST = $(addprefix $(OBJ_DIR)/,$(patsubst %.c,%.o,$(SRC_C_LIST)))
DEPENDENCY_LIST = $(addprefix $(OBJ_DIR)/,$(patsubst %.c,%.d,$(SRC_C_LIST)))

SRC_S_O = $(patsubst %.S,%.o,$(SRC_S))
SRC_S_LIST = $(notdir $(SRC_S)) 
OBJ_S_LIST = $(addprefix $(OBJ_DIR)/,$(patsubst %.S,%.o,$(SRC_S_LIST)))
DEPENDENCY_S_LIST = $(addprefix $(OBJ_DIR)/,$(patsubst %.S,%.d,$(SRC_S_LIST)))

# Compile options
# -------------------------------------------------------------------
CFLAGS =
CFLAGS += -g -marm -mcpu=arm968e-s -march=armv5te -mthumb-interwork -mlittle-endian -Wall -Os -std=c99 -ffunction-sections -Wall -fsigned-char -fdata-sections -Wunknown-pragmas

ASMFLAGS = 
ASMFLAGS += -g -marm -mcpu=arm968e-s -march=armv5te -x assembler-with-cpp

LFLAGS = 
LFLAGS += -g -nostdlib -Wl,--gc-sections -marm -mcpu=arm968e-s


LIBFLAGS =
LIBFLAGS += -L./beken378/lib/ -lrwnx

	
# Compile
# -------------------------------------------------------------------
# add tuya iot lib compile support
# -------------------------------------------------------------------
ROOT_DIR = $(shell pwd)
include project/tuya_iot/build/build_param
TY_SRC_C_LIST = $(notdir $(TY_SRCS))
TY_OBJ_LIST = $(addprefix $(OBJ_DIR)/,$(patsubst %.c,%.o,$(TY_SRC_C_LIST)))
TY_DEPENDENCY_LIST = $(addprefix $(OBJ_DIR)/,$(patsubst %.c,%.d,$(TY_SRC_C_LIST)))
LIBFLAGS += $(TY_IOT_LIB_LDFLAGS)
CFLAGS += $(TY_IOT_CFLAGS)
CFLAGS += -DUSER_SW_VER=\"$(USER_SW_VER)\" -DAPP_BIN_NAME=\"$(APP_BIN_NAME)\"

$(TY_IOT_LIB):$(TY_OBJS) $(TY_OBJ_LIST)
	@mkdir -p $(dir $(TY_IOT_LIB))
	@$(AR) -rcs $(TY_IOT_LIB) $(TY_OBJ_LIST)

# -------------------------------------------------------------------
# add tuya iot application compile support
# -------------------------------------------------------------------
APP_BIN_NAME ?= sample
USER_SW_VER ?= 9.9.9
TY_OUTPUT = project/tuya_user/$(APP_BIN_NAME)/output/$(USER_SW_VER)
TY_SRC_DIRS += $(shell find project/tuya_common/src -type d)
TY_SRC_DIRS += $(shell find project/tuya_user/$(APP_BIN_NAME)/src -type d)
SRC_C += $(foreach dir, $(TY_SRC_DIRS), $(wildcard $(dir)/*.c)) # need export
SRC_C += $(foreach dir, $(TY_SRC_DIRS), $(wildcard $(dir)/*.cpp)) 
SRC_C += $(foreach dir, $(TY_SRC_DIRS), $(wildcard $(dir)/*.s)) 
SRC_C += $(foreach dir, $(TY_SRC_DIRS), $(wildcard $(dir)/*.S)) 

TY_INC_DIRS += $(shell find project/tuya_common/include -type d)
TY_INC_DIRS += $(shell find project/tuya_user/$(APP_BIN_NAME)/include -type d)
INCLUDES += $(foreach base_dir, $(TY_INC_DIRS), $(addprefix -I , $(base_dir))) 

$(TY_OBJS): %.o : %.c
	echo "build $@ ..."
	@$(CC) $(CFLAGS) $(TY_IOT_CFLAGS) $(INCLUDES) -c $< -o $@
	@$(CC) $(CFLAGS) $(TY_IOT_CFLAGS) $(INCLUDES) -c $< -MM -MT $@ -MF $(OBJ_DIR)/$(notdir $(patsubst %.o,%.d,$@))
	@cp $@ $(OBJ_DIR)/$(notdir $@)
	@chmod 777 $(OBJ_DIR)/$(notdir $@)

sinclude $(TY_DEPENDENCY_LIST)

CUR_PATH = $(shell pwd)	
.PHONY: application
application: prerequirement build_info $(SRC_O) $(SRC_S_O) $(TY_IOT_LIB)
ifeq ("${ota_idx}", "1")
	$(LD) $(LFLAGS) -o $(TY_OUTPUT)/$(APP_BIN_NAME)_$(USER_SW_VER).axf  $(OBJ_LIST)  $(OBJ_S_LIST) $(LIBFLAGS) -T./beken378/build/bk7231_ota.ld
else ifeq ("${ota_idx}", "2")
else
	@echo ===========================================================
	@echo ota_idx must be "1" or "2"
	@echo ===========================================================
endif

	$(NM) $(TY_OUTPUT)/$(APP_BIN_NAME)_$(USER_SW_VER).axf | sort > $(TY_OUTPUT)/$(APP_BIN_NAME)_$(USER_SW_VER).map
	$(OBJDUMP) -d $(TY_OUTPUT)/$(APP_BIN_NAME)_$(USER_SW_VER).axf > $(TY_OUTPUT)/$(APP_BIN_NAME)_$(USER_SW_VER).asm
	$(OBJCOPY) -O binary $(TY_OUTPUT)/$(APP_BIN_NAME)_$(USER_SW_VER).axf $(TY_OUTPUT)/$(APP_BIN_NAME)_$(USER_SW_VER).bin
# Generate build info
# -------------------------------------------------------------------	

.PHONY: build_info
build_info:
	echo build_info0
	@echo \#define UTS_VERSION \"`date +%Y/%m/%d-%T`\" > .ver
	@echo \#define RTL8195AFW_COMPILE_TIME \"`date +%Y/%m/%d-%T`\" >> .ver
	@echo \#define RTL8195AFW_COMPILE_DATE \"`date +%Y%m%d`\" >> .ver
	@echo \#define RTL8195AFW_COMPILE_BY \"`id -u -n`\" >> .ver
	@echo \#define RTL8195AFW_COMPILE_HOST \"`$(HOSTNAME_APP)`\" >> .ver
	@if [ -x /bin/dnsdomainname ]; then \
		echo \#define RTL8195AFW_COMPILE_DOMAIN \"`dnsdomainname`\"; \
	elif [ -x /bin/domainname ]; then \
		echo \#define RTL8195AFW_COMPILE_DOMAIN \"`domainname`\"; \
	else \
		echo \#define RTL8195AFW_COMPILE_DOMAIN ; \
	fi >> .ver
	echo build_info1
	@echo \#define RTL195AFW_COMPILER \"gcc `$(CC) $(CFLAGS) -dumpversion | tr --delete '\r'`\" >> .ver
	echo build_info2

.PHONY: prerequirement
prerequirement:
	echo prerequirement0
	@echo ===========================================================
	@echo Build $(APP_BIN_NAME)
	@echo ===========================================================
	echo prerequirement1
	mkdir -p $(OBJ_DIR)
	mkdir -p $(BIN_DIR)
	echo prerequirement2
	
	@# add tuya bin output
	@mkdir -p $(TY_OUTPUT)

$(SRC_O): %.o : %.c
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -MM -MT $@ -MF $(OBJ_DIR)/$(notdir $(patsubst %.o,%.d,$@))
	@cp $@ $(OBJ_DIR)/$(notdir $@)
	@chmod 777 $(OBJ_DIR)/$(notdir $@)

$(SRC_S_O): %.o : %.S
	@$(CC) $(ASMFLAGS) $(INCLUDES) -c $< -o $@
	@$(CC) $(ASMFLAGS) $(INCLUDES) -c $< -MM -MT $@ -MF $(OBJ_DIR)/$(notdir $(patsubst %.o,%.d,$@))
	@cp $@ $(OBJ_DIR)/$(notdir $@)
	@chmod 777 $(OBJ_DIR)/$(notdir $@)

-include $(DEPENDENCY_LIST)
-include $(DEPENDENCY_S_LIST)

# -------------------------------------------------------------------	
# Generate build info
# -------------------------------------------------------------------	
.PHONY: setup
setup:
	@echo "----------------"
	@echo Setup $(GDB_SERVER)
	@echo "----------------"
	
.PHONY: debug
debug:
	@if [ ! -f $(FLASH_TOOLDIR)/rtl_gdb_debug.txt ] ; then echo Please do \"make setup GDB_SERVER=[jlink or openocd]\" first; echo && false ; fi
ifeq ($(findstring CYGWIN, $(OS)), CYGWIN) 
	$(FLASH_TOOLDIR)/Check_Jtag.sh
	cmd /c start $(GDB) -x $(FLASH_TOOLDIR)/rtl_gdb_debug.txt 
else
	$(GDB) -x $(FLASH_TOOLDIR)/rtl_gdb_debug.txt
endif

.PHONY: clean
clean:
	rm -rf $(TARGET)
	rm -f $(SRC_O)
	rm -f $(SRC_S_O)
	rm -rf $(TY_OBJS)
	#rm -f $(TY_IOT_LIB)
	rm -rf $(TY_OUTPUT)
	
