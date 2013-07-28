#include <stdio.h>
#include <string>
#include <vector>
#include <Python.h>

using namespace std;
#include "util.h"
#include "base58.h"

// Need to import key.cpp to be static
#include "key.cpp"

bool fTestNet = false;
const string strMessageMagic = "Bitcoin Signed Message:\n"; // from main.cpp

static PyObject* validate_address(PyObject* self, PyObject* args) {
    const char* charaddress;

    if(!PyArg_ParseTuple(args, "s", &charaddress))
        return NULL;

    CBitcoinAddress address(charaddress);
    bool isValid = address.IsValid();

    return PyBool_FromLong(isValid);
}

static PyObject* verify_message(PyObject* self, PyObject* args) {
    const char* address;
    const char* sign;
    const char* message;

    if(!PyArg_ParseTuple(args, "sss", &address, &sign, &message))
        return NULL;

    string strAddress(address);
    string strSign(sign);
    string strMessage(message);

    CBitcoinAddress addr(strAddress);
    if (!addr.IsValid()) {
        fprintf(stderr, "Invalid address\n");
        Py_RETURN_FALSE;
    }

    CKeyID keyID;
    if (!addr.GetKeyID(keyID)) {
        fprintf(stderr, "Address does not refer to key\n");
        Py_RETURN_FALSE;
    }

    bool fInvalid = false;
    vector<unsigned char> vchSig = DecodeBase64(strSign.c_str(), &fInvalid);

    if (fInvalid) {
        fprintf(stderr, "Malformed base64 encoding\n");
        Py_RETURN_FALSE;
    }

    CHashWriter ss(0);
    ss << strMessageMagic;
    ss << strMessage;

    CKey key;
    if (!key.SetCompactSignature(ss.GetHash(), vchSig)) {
        fprintf(stderr, "Error reading signature\n");
        Py_RETURN_FALSE;
    }

    // If the recovered keyID == the keyID of the address, return True
    if (key.GetPubKey().GetID() == keyID) {
        Py_RETURN_TRUE;
    } else {
        printf("fail\n");
        Py_RETURN_FALSE;
    }

    Py_RETURN_FALSE;
}

static PyObject* sign_message(PyObject* self, PyObject* args) {

    const char* secret; //base64
    const char* message;

    if(!PyArg_ParseTuple(args, "ss", &secret, &message))
        return NULL;

    string strSecret(secret);
    string strMessage(message);

    CBitcoinSecret vchSecret;
    bool fGood = vchSecret.SetString(strSecret);

    if (!fGood) {
        printf("Private key is not good\n");
        Py_RETURN_FALSE;
    }

    CKey key;
    bool fCompressed;
    CSecret csecret = vchSecret.GetSecret(fCompressed);
    key.SetSecret(csecret, fCompressed);

    CHashWriter ss(0);
    ss << strMessageMagic;
    ss << strMessage;

    vector<unsigned char> vchSig;
    if (!key.SignCompact(ss.GetHash(), vchSig)) {
        printf("Sign failed\n");
        Py_RETURN_FALSE;
    }

    return Py_BuildValue("s", EncodeBase64(&vchSig[0], vchSig.size()).c_str());
}
 
static PyMethodDef SignMethods[] =
{
     {"verify_message", verify_message, METH_VARARGS, "verify_message(BitcoinAddress, Signature, Message)"},
     {"sign_message", sign_message, METH_VARARGS, "sign_message(Base58PrivateKey, Message)"},
     {"validate_address", validate_address, METH_VARARGS, "validate_address(BitcoinAddress)"},
     
     {NULL, NULL, 0, NULL}
};
 
PyMODINIT_FUNC initbitcoinsign(void)
{
     (void) Py_InitModule("bitcoinsign", SignMethods);
}