/*
 * optee-security-test
 * Copyright (c) 2023 Peter Nebe <mail@peter-nebe.dev>
 *
 * This file is part of optee-security-test.
 *
 * optee-security-test is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * optee-security-test is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with optee-security-test.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "mock-ta.h"
#include "tee_client_api.h"
#include <iostream>
using namespace std;

int main()
{
  TEEC_Context ctx;
  TEEC_Result res = TEEC_InitializeContext(NULL, &ctx);
  if (res != TEEC_SUCCESS)
    cerr << "TEEC_InitializeContext failed with code 0x" << hex << res << endl;

  TEEC_Session sess;
  const TEEC_UUID uuid = MOCK_TA_UUID;
  uint32_t err_origin;
  res = TEEC_OpenSession(&ctx, &sess, &uuid, TEEC_LOGIN_PUBLIC, NULL, NULL, &err_origin);
  if (res != TEEC_SUCCESS)
    cerr << "TEEC_OpenSession failed with code 0x" << hex << res << " origin 0x" << err_origin << endl;

  TEEC_Operation op{};
  op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INOUT, TEEC_NONE, TEEC_NONE, TEEC_NONE);
  op.params[0].value.a = 123;
  res = TEEC_InvokeCommand(&sess, MOCK_TA_CMD_CHANGE_VALUE, &op, &err_origin);
  if (res != TEEC_SUCCESS)
    cerr << "TEEC_InvokeCommand failed with code 0x" << hex << res << " origin 0x" << err_origin << endl;

  cout << "value after invocation: " << op.params[0].value.a << endl;

  TEEC_CloseSession(&sess);
  TEEC_FinalizeContext(&ctx);

  return 0;
}
