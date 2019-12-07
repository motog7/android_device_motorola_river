/*
   Copyright (c) 2014, The Linux Foundation. All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are
   met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above
      copyright notice, this list of conditions and the following
      disclaimer in the documentation and/or other materials provided
      with the distribution.
    * Neither the name of The Linux Foundation nor the names of its
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
   WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
   ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
   BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
   BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
   WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
   OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
   IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdlib.h>
#include <android-base/properties.h>
#include "vendor_init.h"
#include "property_service.h"
#include <sys/sysinfo.h>
#define _REALLY_INCLUDE_SYS__SYSTEM_PROPERTIES_H_
#include <sys/_system_properties.h>

using android::base::GetProperty;
using android::init::property_set;

void property_override(char const prop[], char const value[])
{
    prop_info *pi;

    pi = (prop_info*) __system_property_find(prop);
    if (pi)
        __system_property_update(pi, value, strlen(value));
    else
        __system_property_add(prop, strlen(prop), value, strlen(value));
}

void property_override_dual(char const system_prop[], char const vendor_prop[], char const value[])
{
    property_override(system_prop, value);
    property_override(vendor_prop, value);
}

void vendor_load_properties()
{
    char carrier2[PROP_VALUE_MAX];
    char mot_clientidbase_am[PROP_VALUE_MAX];
    char clientidbase_am[PROP_VALUE_MAX];
    char mot_clientidbase_ms[PROP_VALUE_MAX];
    char clientidbase_ms[PROP_VALUE_MAX];
    char customerid[PROP_VALUE_MAX];
    char description[PROP_VALUE_MAX];
    char device[PROP_VALUE_MAX];
    char fingerprint[PROP_VALUE_MAX];

    std::string platform = GetProperty("ro.board.platform","");
    if (platform != ANDROID_TARGET)
        return;

    // Warning-less way of sprintf(var, "");
    std::string carrier = GetProperty("ro.boot.carrier","");
    std::string sku = GetProperty("ro.boot.hardware.sku","");

    if (sku == "XT1962-1") {
        /* XT1962-1 */
        if (carrier == "retus") {
            sprintf(carrier2, "retus");
            sprintf(customerid, "retail");
            sprintf(mot_clientidbase_am, "android-motorola");
            sprintf(clientidbase_am, "android-motorola");
            sprintf(mot_clientidbase_ms, "android-motorola-gfw");
            sprintf(clientidbase_ms, "android-motorola-rev2");
        } else if (carrier == "fi") {
            sprintf(carrier2, "fi");
            sprintf(customerid, "retail");
            sprintf(clientidbase_am, "");
            sprintf(mot_clientidbase_am, "");
            sprintf(clientidbase_ms, "android-fi");
            sprintf(mot_clientidbase_ms, "android-fi");
            property_set("ro.cdma.home.operator.alpha", "Google Fi");
            property_set("gsm.operator.alpha", "Google Fi");
            property_set("gsm.sim.operator.alpha", "Google Fi");
            property_set("persist.radio.fi_supported", "1");
            property_set("persist.vendor.radio.fi_supported", "1");
        }

        property_set("ro.mot.gms.clientidbase", "android-motorola");
        property_set("ro.com.google.clientidbase", "android-motorola");
        property_set("ro.com.google.rlz_ap_whitelist", "y0,y5,y6,y7,y8,YH");
        property_set("ro.com.google.rlzbrandcode", "MOTC");
        property_set("gsm.operator.iso-country", "us");
        property_set("persist.radio.pb.min.match", "7");
        property_set("gsm.defaultpdpcontext.active", "true");
        property_set("persist.radio.cdma.msgid", "2");
        property_set("telephony.lteOnCdmaDevice", "1");
        property_set("gsm.defaultpdpcontext.active", "true");
    }

    sprintf(description, "[river-user 9 PPOS29.114-134-2 4595e release-keys");
    sprintf(fingerprint, "motorola/river/river:9/PPOS29.114-134-2/4595e:user/release-keys");

    property_override_dual("ro.carrier", "ro.vendor.carrier", carrier2);
    property_override_dual("ro.build.fingerprint", "ro.vendor.product.device", device);
    property_override_dual("ro.build.product", "ro.vendor.build.product", device);
    property_override_dual("ro.build.description", "ro.vendor.build.description", description);
    property_override_dual("ro.build.fingerprint", "ro.vendor.build.fingerprint", fingerprint);

    property_set("ro.mot.gms.clientidbase.am", mot_clientidbase_am);
    property_set("ro.com.google.clientidbase.am", clientidbase_am);
    property_set("ro.mot.gms.clientidbase.ms", mot_clientidbase_ms);
    property_set("ro.com.google.clientidbase.ms", clientidbase_ms);

    property_set("ro.mot.build.customerid", customerid);
}
