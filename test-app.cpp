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
#include <stdexcept>
#include <iostream>
using namespace std;

class TeeClient
{
  TEEC_Context context;

public:
  TeeClient()
  {
    TEEC_Result res = TEEC_InitializeContext(NULL, &context);
    if (res != TEEC_SUCCESS)
      throw runtime_error("TEEC_InitializeContext");
  }

  ~TeeClient()
  {
    TEEC_FinalizeContext(&context);
  }

  bool invokeCommand(uint32_t commandId, uint32_t &value)
  {
    TEEC_Session session;
    const TEEC_UUID uuid = MOCK_TA_UUID;
    uint32_t errOrigin;
    TEEC_Result res = TEEC_OpenSession(&context, &session, &uuid, TEEC_LOGIN_PUBLIC, NULL, NULL, &errOrigin);
    if (res != TEEC_SUCCESS)
    {
      cerr << "TEEC_OpenSession failed with code 0x" << hex << res << " origin 0x" << errOrigin << endl;
      return false;
    }

    TEEC_Operation op{};
    op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INOUT, TEEC_NONE, TEEC_NONE, TEEC_NONE);
    op.params[0].value.a = value;
    res = TEEC_InvokeCommand(&session, commandId, &op, &errOrigin);
    if (res != TEEC_SUCCESS)
      cerr << "TEEC_InvokeCommand failed with code 0x" << hex << res << " origin 0x" << errOrigin << endl;

    value = op.params[0].value.a;
    TEEC_CloseSession(&session);

    return res == TEEC_SUCCESS;
  }
};

int main()
{
  TeeClient teeClient;
  uint32_t value = 123;

  bool success = teeClient.invokeCommand(MOCK_TA_CMD_CHANGE_VALUE, value);
  cout << "success: " << boolalpha << success;
  cout << ", value after invocation: " << value << endl;

  return 0;
}
