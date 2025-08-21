.attribute arch, "rv64im_zicsr"

.globl get_fcsr
get_fcsr:
    csrr a0,fcsr
    ret

.globl get_mstatus
get_mstatus:
    csrr a0,mstatus
    ret

.globl enable_fp
enable_fp:
    li a0, 1 << 13
    csrs mstatus,a0
    ret