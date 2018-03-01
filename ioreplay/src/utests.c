// Copyright 2018 Mimecast Ltd.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "utests.h"

#include "datas/amap.h"
#include "datas/hmap.h"
#include "datas/list.h"
#include "datas/rbuffer.h"

void utests_run()
{
    fprintf(stderr, "Running unit tests\n");
    amap_test();
    hmap_test();
    list_test();
    rbuffer_test();
    fprintf(stderr, "Great success, run all unit tests without any errors!\n");
}
