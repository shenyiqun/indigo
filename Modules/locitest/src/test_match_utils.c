/****************************************************************
 *
 *        Copyright 2013, Big Switch Networks, Inc. 
 * 
 * Licensed under the Eclipse Public License, Version 1.0 (the
 * "License"); you may not use this file except in compliance
 * with the License. You may obtain a copy of the License at
 * 
 *        http://www.eclipse.org/legal/epl-v10.html
 * 
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the
 * License.
 *
 ****************************************************************/

/* Copyright 2013, Big Switch Networks, Inc. */

/**
 *
 * Match utility test cases
 *
 */

#include <locitest/test_common.h>

int
test_match_utils(void)
{
    of_mac_addr_t m1 = {{0,0,1,1,3,3}};
    of_mac_addr_t m2 = {{0,0,1,1,3,3}};  /* m1 == m2 */
    of_mac_addr_t m3 = {{0,0,1,1,1,1}};  /* m1 is more specific than m3 */
    of_mac_addr_t m4 = {{0xf,0,1,1,3,3}};  /* m1 is not more specific m4 */
    of_mac_addr_t m5 = {{0,0,1,1,3,0xf}};  /* m1 is not more specific m5 */

    of_mac_addr_t m_mask1 = {{0xff, 0xff, 0xff, 0xff, 0xff, 0xff}};
    of_mac_addr_t m_mask2 = {{0xff, 0xff, 0xff, 0xff, 0, 0}};

    /* m1 matches m2 for mask1 */
    /* m1 matches m2 for mask2 */
    /* m1 does not match m3, m4, m5 for mask1 */
    /* m1 matches m3, m5 for mask2 */
    /* m1 does not match m4 for mask2 */

    of_ipv6_t i1 = {{0,0,0,0,1,1,1,1,3,3,3,3,7,7,7,7}};  /* same as above */
    of_ipv6_t i2 = {{0,0,0,0,1,1,1,1,3,3,3,3,7,7,7,7}};
    of_ipv6_t i3 = {{0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1}};
    of_ipv6_t i4 = {{0xf,0,0,0,1,1,1,1,3,3,3,3,7,7,7,7}};
    of_ipv6_t i5 = {{0,0,0,0,1,1,1,1,3,3,3,3,7,7,7,0xf}};

    of_ipv6_t i_mask1 = {{0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                          0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff}};
    of_ipv6_t i_mask2 = {{0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                          0, 0, 0, 0, 0, 0, 0, 0}};

    /* Same relationships as above */

    uint32_t v1 = 3;  /* same as above */
    uint32_t v2 = 3;
    uint32_t v3 = 1;
    uint32_t v4 = 4;
    uint32_t v5 = (1 << 31) | 1;

    uint32_t u32_mask1 = -1;
    uint32_t u32_mask2 = 1;

    uint64_t w1 = 3;  /* same as above */
    uint64_t w2 = 3;
    uint64_t w3 = 1;
    uint64_t w4 = 4;
    uint64_t w5 = (1LL << 63) | 1LL;

    uint64_t u64_mask1 = -1;
    uint64_t u64_mask2 = 1;

    /* Match structures */
    of_match_t match1, match2;

    TEST_ASSERT(OF_MORE_SPECIFIC_MAC_ADDR(&m1, &m2));
    TEST_ASSERT(OF_MORE_SPECIFIC_MAC_ADDR(&m1, &m3));
    TEST_ASSERT(!OF_MORE_SPECIFIC_MAC_ADDR(&m1, &m4));
    TEST_ASSERT(!OF_MORE_SPECIFIC_MAC_ADDR(&m1, &m5));

    TEST_ASSERT(OF_MORE_SPECIFIC_IPV6(&i1, &i2));
    TEST_ASSERT(OF_MORE_SPECIFIC_IPV6(&i1, &i3));
    TEST_ASSERT(!OF_MORE_SPECIFIC_IPV6(&i1, &i4));
    TEST_ASSERT(!OF_MORE_SPECIFIC_IPV6(&i1, &i5));

    TEST_ASSERT(OF_MORE_SPECIFIC_INT(v1, v2));
    TEST_ASSERT(OF_MORE_SPECIFIC_INT(v1, v3));
    TEST_ASSERT(!OF_MORE_SPECIFIC_INT(v1, v4));
    TEST_ASSERT(!OF_MORE_SPECIFIC_INT(v1, v5));

    TEST_ASSERT(OF_MORE_SPECIFIC_INT(w1, w2));
    TEST_ASSERT(OF_MORE_SPECIFIC_INT(w1, w3));
    TEST_ASSERT(!OF_MORE_SPECIFIC_INT(w1, w4));
    TEST_ASSERT(!OF_MORE_SPECIFIC_INT(w1, w5));

    /* Test restricted matches on macs */
    TEST_ASSERT(OF_RESTRICTED_MATCH_MAC_ADDR(&m1, &m2, &m_mask1));
    TEST_ASSERT(!OF_RESTRICTED_MATCH_MAC_ADDR(&m1, &m3, &m_mask1));
    TEST_ASSERT(!OF_RESTRICTED_MATCH_MAC_ADDR(&m1, &m4, &m_mask1));
    TEST_ASSERT(!OF_RESTRICTED_MATCH_MAC_ADDR(&m1, &m5, &m_mask1));

    TEST_ASSERT(OF_RESTRICTED_MATCH_MAC_ADDR(&m1, &m2, &m_mask2));
    TEST_ASSERT(OF_RESTRICTED_MATCH_MAC_ADDR(&m1, &m3, &m_mask2));
    TEST_ASSERT(!OF_RESTRICTED_MATCH_MAC_ADDR(&m1, &m4, &m_mask2));
    TEST_ASSERT(OF_RESTRICTED_MATCH_MAC_ADDR(&m1, &m5, &m_mask2));

    /* Test overlap */
    TEST_ASSERT(OF_OVERLAP_MAC_ADDR(&m1, &m2, &m_mask1, &m_mask2));
    TEST_ASSERT(OF_OVERLAP_MAC_ADDR(&m1, &m3, &m_mask1, &m_mask2));
    TEST_ASSERT(!OF_OVERLAP_MAC_ADDR(&m1, &m3, &m_mask1, &m_mask1));
    TEST_ASSERT(!OF_OVERLAP_MAC_ADDR(&m1, &m4, &m_mask1, &m_mask2));
    TEST_ASSERT(OF_OVERLAP_MAC_ADDR(&m1, &m5, &m_mask1, &m_mask2));

    /* Test restricted matches on ipv6 */
    TEST_ASSERT(OF_RESTRICTED_MATCH_IPV6(&i1, &i2, &i_mask1));
    TEST_ASSERT(!OF_RESTRICTED_MATCH_IPV6(&i1, &i3, &i_mask1));
    TEST_ASSERT(!OF_RESTRICTED_MATCH_IPV6(&i1, &i4, &i_mask1));
    TEST_ASSERT(!OF_RESTRICTED_MATCH_IPV6(&i1, &i5, &i_mask1));

    TEST_ASSERT(OF_RESTRICTED_MATCH_IPV6(&i1, &i2, &i_mask2));
    TEST_ASSERT(OF_RESTRICTED_MATCH_IPV6(&i1, &i3, &i_mask2));
    TEST_ASSERT(!OF_RESTRICTED_MATCH_IPV6(&i1, &i4, &i_mask2));
    TEST_ASSERT(OF_RESTRICTED_MATCH_IPV6(&i1, &i5, &i_mask2));

    /* Test overlap */
    TEST_ASSERT(OF_OVERLAP_IPV6(&i1, &i2, &i_mask1, &i_mask2));
    TEST_ASSERT(OF_OVERLAP_IPV6(&i1, &i3, &i_mask1, &i_mask2));
    TEST_ASSERT(!OF_OVERLAP_IPV6(&i1, &i3, &i_mask1, &i_mask1));
    TEST_ASSERT(!OF_OVERLAP_IPV6(&i1, &i4, &i_mask1, &i_mask2));
    TEST_ASSERT(OF_OVERLAP_IPV6(&i1, &i5, &i_mask1, &i_mask2));

    /* Test restricted matches on uint32 */
    TEST_ASSERT(OF_RESTRICTED_MATCH_INT(v1, v2, u32_mask1));
    TEST_ASSERT(!OF_RESTRICTED_MATCH_INT(v1, v3, u32_mask1));
    TEST_ASSERT(!OF_RESTRICTED_MATCH_INT(v1, v4, u32_mask1));
    TEST_ASSERT(!OF_RESTRICTED_MATCH_INT(v1, v5, u32_mask1));

    TEST_ASSERT(OF_RESTRICTED_MATCH_INT(v1, v2, u32_mask2));
    TEST_ASSERT(OF_RESTRICTED_MATCH_INT(v1, v3, u32_mask2));
    TEST_ASSERT(!OF_RESTRICTED_MATCH_INT(v1, v4, u32_mask2));
    TEST_ASSERT(OF_RESTRICTED_MATCH_INT(v1, v5, u32_mask2));

    /* Test overlap */
    TEST_ASSERT(OF_OVERLAP_INT(v1, v2, u32_mask1, u32_mask2));
    TEST_ASSERT(OF_OVERLAP_INT(v1, v3, u32_mask1, u32_mask2));
    TEST_ASSERT(!OF_OVERLAP_INT(v1, v3, u32_mask1, u32_mask1));
    TEST_ASSERT(!OF_OVERLAP_INT(v1, v4, u32_mask1, u32_mask2));
    TEST_ASSERT(OF_OVERLAP_INT(v1, v5, u32_mask1, u32_mask2));

    /* Test restricted matches on uint64 */
    TEST_ASSERT(OF_RESTRICTED_MATCH_INT(w1, w2, u64_mask1));
    TEST_ASSERT(!OF_RESTRICTED_MATCH_INT(w1, w3, u64_mask1));
    TEST_ASSERT(!OF_RESTRICTED_MATCH_INT(w1, w4, u64_mask1));
    TEST_ASSERT(!OF_RESTRICTED_MATCH_INT(w1, w5, u64_mask1));

    TEST_ASSERT(OF_RESTRICTED_MATCH_INT(w1, w2, u64_mask2));
    TEST_ASSERT(OF_RESTRICTED_MATCH_INT(w1, w3, u64_mask2));
    TEST_ASSERT(!OF_RESTRICTED_MATCH_INT(w1, w4, u64_mask2));
    TEST_ASSERT(OF_RESTRICTED_MATCH_INT(w1, w5, u64_mask2));

    /* Test overlap */
    TEST_ASSERT(OF_OVERLAP_INT(w1, w2, u64_mask1, u64_mask2));
    TEST_ASSERT(OF_OVERLAP_INT(w1, w3, u64_mask1, u64_mask2));
    TEST_ASSERT(!OF_OVERLAP_INT(w1, w3, u64_mask1, u64_mask1));
    TEST_ASSERT(!OF_OVERLAP_INT(w1, w4, u64_mask1, u64_mask2));
    TEST_ASSERT(OF_OVERLAP_INT(w1, w5, u64_mask1, u64_mask2));

    /* Test match stuctures */
    of_match_populate(&match1, OF_VERSION_1_2, 1);
    of_match_populate(&match2, OF_VERSION_1_2, 1);
    TEST_ASSERT(of_match_eq(&match1, &match2));
    TEST_ASSERT(of_match_eq(&match2, &match1));
    TEST_ASSERT(of_match_more_specific(&match1, &match2));
    TEST_ASSERT(of_match_more_specific(&match2, &match1));
    TEST_ASSERT(of_match_overlap(&match1, &match2));
    TEST_ASSERT(of_match_overlap(&match2, &match1));

    /* Change match2 so it still is extended by match1 */
    memset(&match2.masks.eth_dst, 0, sizeof(of_mac_addr_t));
    TEST_ASSERT(of_match_more_specific(&match1, &match2));
    TEST_ASSERT(!of_match_more_specific(&match2, &match1));
    TEST_ASSERT(!of_match_eq(&match1, &match2));
    TEST_ASSERT(of_match_overlap(&match1, &match2));
    TEST_ASSERT(of_match_overlap(&match2, &match1));

    /* Now change a value so that matches disagree */
    match2.fields.in_port++;
    TEST_ASSERT(!of_match_more_specific(&match1, &match2));
    TEST_ASSERT(!of_match_overlap(&match1, &match2));
    TEST_ASSERT(!of_match_overlap(&match2, &match1));
    /* Clear inport mask on match2 and should extend again */
    match2.masks.in_port = 0;
    TEST_ASSERT(of_match_more_specific(&match1, &match2));
    TEST_ASSERT(of_match_overlap(&match1, &match2));
    TEST_ASSERT(of_match_overlap(&match2, &match1));

    /* Now change mask so the overlap, but not more specific */
    match1.fields.in_port = 0x7;
    match1.masks.in_port = 0x7;
    match2.fields.in_port = 0xe;
    match2.masks.in_port = 0xe;
    TEST_ASSERT(!of_match_more_specific(&match1, &match2));
    TEST_ASSERT(!of_match_more_specific(&match2, &match1));
    TEST_ASSERT(of_match_overlap(&match1, &match2));
    TEST_ASSERT(of_match_overlap(&match2, &match1));

    return TEST_PASS;
}