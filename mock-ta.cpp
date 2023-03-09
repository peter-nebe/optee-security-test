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
extern "C" {
#include "tee_internal_api.h"
}

TEE_Result TA_CreateEntryPoint()
{
  return TEE_SUCCESS;
}

void TA_DestroyEntryPoint()
{
}

TEE_Result TA_OpenSessionEntryPoint(uint32_t paramTypes, TEE_Param __maybe_unused params[4], void __maybe_unused **sess_ctx)
{
  if (paramTypes != TEE_PARAM_TYPES(TEE_PARAM_TYPE_NONE, TEE_PARAM_TYPE_NONE, TEE_PARAM_TYPE_NONE, TEE_PARAM_TYPE_NONE))
    return TEE_ERROR_BAD_PARAMETERS;

  return TEE_SUCCESS;
}

void TA_CloseSessionEntryPoint(void __maybe_unused *sess_ctx)
{
}

static TEE_Result changeValue(uint32_t paramTypes, TEE_Param params[4])
{
  if (paramTypes != TEE_PARAM_TYPES(TEE_PARAM_TYPE_VALUE_INOUT, TEE_PARAM_TYPE_NONE, TEE_PARAM_TYPE_NONE, TEE_PARAM_TYPE_NONE))
    return TEE_ERROR_BAD_PARAMETERS;

  IMSG("Got value: %u from NW", params[0].value.a);
  params[0].value.a *= 10;
  IMSG("Change value to: %u", params[0].value.a);

  return TEE_SUCCESS;
}

TEE_Result TA_InvokeCommandEntryPoint(void __maybe_unused *sess_ctx, uint32_t cmdId, uint32_t paramTypes, TEE_Param params[4])
{
  switch (cmdId)
  {
    case MOCK_TA_CMD_CHANGE_VALUE:
      return changeValue(paramTypes, params);
    default:
      return TEE_ERROR_BAD_PARAMETERS;
  }
}
