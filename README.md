# Requirements
- `bpftools`

# Setps
1. Generate `vmlinux.h` using
   ```
   $ bpftool btf dump file /sys/kernel/btf/vmlinux format c > ./vmlinux.h
   ```
2. Build
   ```
   $ make
   ```
3. Run
   ```
   # ./loader
   ```
