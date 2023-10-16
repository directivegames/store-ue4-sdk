// Copyright 2023 Xsolla Inc. All Rights Reserved.

package com.xsolla.login;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;

import androidx.annotation.Nullable;

import com.xsolla.android.login.XLogin;
import com.xsolla.android.login.callback.FinishSocialCallback;
import com.xsolla.android.login.callback.StartSocialCallback;
import com.xsolla.android.login.social.SocialNetwork;

public class XsollaNativeAuthActivity extends Activity {
    public static String ARG_SOCIAL_NETWORK = "social_network";
	public static String REMEMBER_ME = "remember_me";
    public static String CALLBACK_ADDRESS = "callback_address";
    public static native void onAuthSuccessCallback(long callback, String accessToken, String refreshToken, long expiresAt, boolean rememberMe);
    public static native void onAuthCancelCallback(long callback);
    public static native void onAuthErrorCallback(long callback, String errorMessage);

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        if (savedInstanceState != null) {
            finish();
            return;
        }

        SocialNetwork socialNetwork = SocialNetwork.valueOf(getIntent().getStringExtra(ARG_SOCIAL_NETWORK));

        XLogin.startSocialAuth(this, socialNetwork, new StartSocialCallback() {
            @Override
            public void onAuthStarted() {
                Log.d("XsollaAuthActivity", "onAuthStarted");
            }

            @Override
            public void onError(Throwable throwable, String s) {
                Log.d("XsollaAuthActivity", "onAuthError: " + s);
                finish();
            }
        });
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);

        SocialNetwork socialNetwork = SocialNetwork.valueOf(getIntent().getStringExtra(ARG_SOCIAL_NETWORK));

        XLogin.finishSocialAuth(this, socialNetwork, requestCode, resultCode, data, new FinishSocialCallback() {
            @Override
            public void onAuthSuccess() {
                Log.d("XsollaAuthActivity", "onAuthSuccess");
                onAuthSuccessCallback(getIntent().getLongExtra(CALLBACK_ADDRESS, 0),
					XLogin.getToken(),
                    XLogin.getRefreshToken(),
                    XLogin.getTokenExpireTime(),
					getIntent().getBooleanExtra(REMEMBER_ME, false));
                finish();
            }

            @Override
            public void onAuthCancelled() {
                Log.d("XsollaAuthActivity", "onAuthCancelled");
                onAuthCancelCallback(getIntent().getLongExtra(CALLBACK_ADDRESS, 0));
                finish();
            }

            @Override
            public void onAuthError(Throwable throwable, String error) {
                Log.d("XsollaAuthActivity", "onAuthError");
                String errorMessage = (error != null && !error.isEmpty()) ? error : "Unknown error";
                onAuthErrorCallback(getIntent().getLongExtra(CALLBACK_ADDRESS, 0), errorMessage);
                finish();
            }
        });
    }
}