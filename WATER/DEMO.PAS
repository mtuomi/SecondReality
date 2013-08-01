uses crt,t1,t2,t3,bkr,miek;

procedure setrgb(c,r,g,b:byte);
begin
     port[$3c8] := c;
     port[$3c9] := r;
     port[$3c9] := g;
     port[$3c9] := b;
end;

var  frames : word;

procedure waitr;
begin
        asm
	mov	bx,1
	int	0fch
        mov     frames,ax
        end;
end;


function dis_exit:boolean;
var   a : byte;
begin
        asm
	mov	bx,2
	int	0fch
        mov     a,al
        end;
        if a = 0 then dis_exit := false else dis_exit := true;
end;

var   f : file;
      inx : word;
      y,x,t   : word;
      tmppal,pal    : array[0..768] of byte;
      font   : array[0..400*35] of byte;
      fbuf   : array[0..158*34] of byte;
      scp,sss:word;

{$L routines.obj}
          { dx = Bground seg,
            ax = pos seg,
            si = pos ofs,
            cx = font seg,
            bx = font ofs }

procedure putrouts1;far;external;

procedure scr(pos:byte);
begin
  case pos of

   0:asm
      mov   dx,seg tausta
      mov   ax,seg wat1
      mov   si,offset wat1
      mov   cx,seg font
      mov   bx,offset fbuf
      call  putrouts1
     end;
   1:asm
      mov   dx,seg tausta
      mov   ax,seg wat2
      mov   si,offset wat2
      mov   cx,seg font
      mov   bx,offset fbuf
      call  putrouts1
     end;
   2:asm
      mov   dx,seg tausta
      mov   ax,seg wat3
      mov   si,offset wat3
      mov   cx,seg font
      mov   bx,offset fbuf
      call  putrouts1
      end;
  end;
end;


var   frame   : word;
      co      : word;
      fadeout : boolean;
      quit    : word;
      fp      : word;
      pf      : word;
begin
     asm
     mov   ax,$13
     int   $10

     xor   bx,bx
     int   0fch


@a:  mov   ax,0
     mov   bx,6
     int   0fch
     cmp   dx,0
     jl    @a
     mov   co,cx

     end;

     fillchar(fbuf,sizeof(fbuf),#0);
     move(mem[seg(_miekka):ofs(_miekka)+10],pal,768);
     move(mem[seg(_miekka):ofs(_miekka)+778],font,400*34);

     for x := 0 to 255 do setrgb(x,0,0,0);
     move(mem[seg(tausta):ofs(tausta)+778],mem[seg(tausta):ofs(tausta)],64000);
     move(mem[seg(tausta):ofs(tausta)],mem[$a000:0],64000);

     move(pal,tmppal,768);
     fillchar(pal,768,#0);
     for y := 0 to 63*2 do
      begin
        waitr;
        if y and 1 = 1 then
        begin
        for x := 0 to 255 do
         begin
          setrgb(x,pal[x*3+0],pal[x*3+1],pal[x*3+2]);
          for pf := 0 to 3 do if pal[x*3+pf] < tmppal[x*3+pf] then inc(pal[x*3+pf]);
         end;
        end;

        scr(sss);
        if sss = 2 then sss := 0 else inc(sss);

      end;


     asm
@a:  mov   ax,0
     mov   bx,6
     int   0fch
     cmp   cx,CO
     je    @a
     cmp   bx,16
     jl    @a
     end;



    {######################################################################}

    sss := 0;
    scp  := 0;
    frame := 0;
    quit  := 0;
    fadeout := false;
    fillchar(tmppal,768,#0);
    repeat
      waitr;

     asm
     mov   ax,0
     mov   bx,6
     int   0fch
     cmp   dx,-11
     jnz   @a
     mov   fadeout,1
@a:  end;


      if fadeout then
        begin
        if fp = 64 then quit := 1 else inc(fp);
        for x := 0 to 255 do
         begin
          setrgb(x,pal[x*3+0],pal[x*3+1],pal[x*3+2]);
          for pf := 0 to 3 do if pal[x*3+pf] > tmppal[x*3+pf] then dec(pal[x*3+pf]);
         end;
        end;


      scr(sss);
      if sss = 2 then
       begin
        sss := 0;
        move(fbuf[1],fbuf,sizeof(fbuf));
        for x := 0 to 33 do fbuf[158+x*158] := font[x*400+scp];
        if scp < 390 then inc(scp);
       end else inc(sss);


     until dis_exit or (quit = 1);
end.
