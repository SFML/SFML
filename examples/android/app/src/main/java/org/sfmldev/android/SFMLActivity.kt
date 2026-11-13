package org.sfmldev.android

import android.app.NativeActivity
import android.os.Bundle
import android.util.Log
import android.view.KeyEvent
import android.view.ViewGroup
import android.view.inputmethod.EditorInfo
import android.view.inputmethod.InputConnection
import android.view.inputmethod.InputConnectionWrapper
import android.widget.EditText

class SFMLActivity : NativeActivity() {

    external fun nativeOnTextInput(text: String)

    private lateinit var imeProxy: EditText

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        Log.i("SFMLActivity", "Created")

        val root = window.decorView.findViewById<ViewGroup>(android.R.id.content)

        imeProxy = object : EditText(this) {
            override fun onCreateInputConnection(outAttrs: EditorInfo): InputConnection {
                val base = super.onCreateInputConnection(outAttrs)
                return object : InputConnectionWrapper(base, true) {
                    override fun commitText(text: CharSequence?, newCursorPosition: Int): Boolean {
                        if (!text.isNullOrEmpty()) {
                            nativeOnTextInput(text.toString())
                        }
                        return super.commitText(text, newCursorPosition)
                    }
                }
            }
        }

        imeProxy.isFocusable = true
        imeProxy.isFocusableInTouchMode = true
        imeProxy.visibility = EditText.INVISIBLE

        root.addView(imeProxy, ViewGroup.LayoutParams(1, 1))
        imeProxy.requestFocus()
    }

    override fun onKeyMultiple(keyCode: Int, repeatCount: Int, event: KeyEvent): Boolean {
        val chars = event.characters
        if (!chars.isNullOrEmpty()) {
            nativeOnTextInput(chars)
            return true
        }
        return super.onKeyMultiple(keyCode, repeatCount, event)
    }
}
