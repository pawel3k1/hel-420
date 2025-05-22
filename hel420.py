import hashlib
import math
import argparse

def integral_mix_function(x: float) -> float:
    """
    Custom mix function inspired by basic integral logic:
    f(x) = (x^sin(x)) * 63^x + 1/x
    Used to transform byte data in a non-reversible way.
    """
    if x == 0:
        x = 1e-10  # avoid division by zero
    try:
        result = (x ** math.sin(x)) * (63 ** x) + (1 / x)
    except OverflowError:
        # prevent crashing on large powers
        result = (x ** math.sin(x)) * 1e10 + (1 / x)
    return result

def mix_data(data: bytes, rounds: int = 3) -> bytes:
    """
    Applies the mix function to each byte, multiple times.
    """
    result = bytearray(data)
    for _ in range(rounds):
        for i in range(len(result)):
            x = result[i] / 255.0 * math.pi  # scale byte to 0–π
            mixed = integral_mix_function(x)
            result[i] ^= int(mixed * 1e5) % 256
    return bytes(result)

def hel_420_hash(input_data: str) -> str:
    """
    Main HEL-420 hashing function.
    Step 1: SHA-256
    Step 2: Mix with custom transformation
    Step 3: Final SHA-256
    """
    data = input_data.encode('utf-8')
    sha_stage1 = hashlib.sha256(data).digest()
    mixed = mix_data(sha_stage1)
    final_hash = hashlib.sha256(mixed).hexdigest()
    return final_hash

def main():
    parser = argparse.ArgumentParser(description="HEL-420 hash generator (SHA-256 with custom mixing)")
    parser.add_argument("-i", "--input", required=True, help="Input string to hash")
    args = parser.parse_args()

    hash_result = hel_420_hash(args.input)
    print(f"HEL-420 hash:\n{hash_result}")

if __name__ == "__main__":
    main()
