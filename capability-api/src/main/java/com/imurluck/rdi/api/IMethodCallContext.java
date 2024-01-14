package com.imurluck.rdi.api;

import androidx.annotation.NonNull;

public interface IMethodCallContext {

    /**
     * @return method arguments, if no arguments, return an empty array
     */
    @NonNull
    Object[] getArguments();
}
