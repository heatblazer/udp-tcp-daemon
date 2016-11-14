#include "plugin-interface.h"
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <netinet/in.h>
#include <netinet/ip.h>
#include <net/if.h>
#include <netinet/if_ether.h>

#include <pcap.h>

// mine threaded app
#include <pthread.h>


/* We've included the UDP header struct for your ease of customization.
 * For your protocol, you might want to look at netinet/tcp.h for hints
 * on how to deal with single bits or fields that are smaller than a byte
 * in length.
 *
 * Per RFC 768, September, 1981.
 */
struct UDP_hdr {
    u_short	uh_sport;		/* source port */
    u_short	uh_dport;		/* destination port */
    u_short	uh_ulen;		/* datagram length */
    u_short	uh_sum;			/* datagram checksum */
};
////////////////////////////////////////////////////////////////////////////////
// defs
static void dump_UDP_packet(const unsigned char *packet, struct timeval ts,
            unsigned int capture_len);



////////////////////////////////////////////////////////////////////////////////

static pthread_t g_worker_thread;

/// thread library
/// \brief worker
/// \param pArgs
/// \return
///
static void* worker(void* pArgs)
{
    (void) pArgs;
    return 0;
}




////////////////////////////////////////////////////////////////////////////////

/* Note, this routine returns a pointer into a static buffer, and
 * so each call overwrites the value returned by the previous call.
 */
static const char *timestamp_string(struct timeval ts)
{
    static char timestamp_string_buf[256];

    sprintf(timestamp_string_buf, "%d.%06d",
        (int) ts.tv_sec, (int) ts.tv_usec);

    return timestamp_string_buf;
}

static void problem_pkt(struct timeval ts, const char *reason)
{
    fprintf(stderr, "%s: %s\n", timestamp_string(ts), reason);
}

static void too_short(struct timeval ts, const char *truncated_hdr)
{
    fprintf(stderr, "packet with timestamp %s is truncated and lacks a full %s\n",
    timestamp_string(ts), truncated_hdr);
}


static void init()
{
    printf("Sniffer init...\n");
    pthread_create(&g_worker_thread, NULL, worker, NULL);
}

int put_ndata(void *data, int len)
{
    (void) data;
    (void) len;
    return 0;
}

static int put_data(void *data)
{
    (void) data;
    return 0;
}

static void *get_data()
{
    return NULL;
}

static void deinit()
{
    printf("Sniffer deinit...\n");
    pthread_join(g_worker_thread, 0);
}

static struct interface_t s_iface;

/* dump_UDP_packet()
 *
 * This routine parses a packet, expecting Ethernet, IP, and UDP headers.
 * It extracts the UDP source and destination port numbers along with the UDP
 * packet length by casting structs over a pointer that we move through
 * the packet.  We can do this sort of casting safely because libpcap
 * guarantees that the pointer will be aligned.
 *
 * The "ts" argument is the timestamp associated with the packet.
 *
 * Note that "capture_len" is the length of the packet *as captured by the
 * tracing program*, and thus might be less than the full length of the
 * packet.  However, the packet pointer only holds that much data, so
 * we have to be careful not to read beyond it.
 */
static void dump_UDP_packet(const unsigned char *packet, struct timeval ts,
            unsigned int capture_len)
{
    struct ip *ip;
    struct UDP_hdr *udp;
    unsigned int IP_header_length;

    /* For simplicity, we assume Ethernet encapsulation. */

    if (capture_len < sizeof(struct ether_header))
        {
        /* We didn't even capture a full Ethernet header, so we
         * can't analyze this any further.
         */
        too_short(ts, "Ethernet header");
        return;
        }

    /* Skip over the Ethernet header. */
    packet += sizeof(struct ether_header);
    capture_len -= sizeof(struct ether_header);

    if (capture_len < sizeof(struct ip))
        { /* Didn't capture a full IP header */
        too_short(ts, "IP header");
        return;
        }

    ip = (struct ip*) packet;
    IP_header_length = ip->ip_hl * 4;	/* ip_hl is in 4-byte words */

    if (capture_len < IP_header_length)
        { /* didn't capture the full IP header including options */
        too_short(ts, "IP header with options");
        return;
        }

    if (ip->ip_p != IPPROTO_UDP)
        {
        problem_pkt(ts, "non-UDP packet");
        return;
        }

    /* Skip over the IP header to get to the UDP header. */
    packet += IP_header_length;
    capture_len -= IP_header_length;

    if (capture_len < sizeof(struct UDP_hdr))
    {
        too_short(ts, "UDP header");
        return;
    }

    udp = (struct UDP_hdr*) packet;

    printf("%s UDP src_port=%d dst_port=%d length=%d\n",
        timestamp_string(ts),
        ntohs(udp->uh_sport),
        ntohs(udp->uh_dport),
        ntohs(udp->uh_ulen));
    }



static int main_proxy(int argc, char** argv)
{
    pcap_t *pcap;
    const unsigned char *packet;
    char errbuf[PCAP_ERRBUF_SIZE];
    struct pcap_pkthdr header;

        /* Skip over the program name. */
    ++argv; --argc;

    pcap = pcap_open_offline(argv[0], errbuf);
    if (pcap == NULL)
    {
        fprintf(stderr, "error reading pcap file: %s\n", errbuf);
        exit(1);
    }
    while ((packet = pcap_next(pcap, &header)) != NULL) {
        dump_UDP_packet(packet, header.ts, header.caplen);
    }

    return 0;
}


const struct interface_t *get_interface()
{
    s_iface.init = &init;
    s_iface.deinit = &deinit;
    s_iface.get_data = &get_data;
    s_iface.put_data = &put_data;
    s_iface.put_ndata = &put_ndata;
    s_iface.main_proxy = &main_proxy;

    return &s_iface;
}
